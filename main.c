/*
	Copyright © 2015,2019 Daniel T. Borelli <danieltborelli@gmail.com>

	This file is part of chkpasswd.

	chkpasswd is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	chkpasswd is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with chkpasswd.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "notify_wrap.h"
#include "pam_auth.h"
#include "desktops.h"
#include "opciones.h"


static GMainLoop *main_loop = NULL;



static void accion_terminada(GObject *object,
			GAsyncResult *res,
			gpointer datos)
{
	(void)object;
	(void)res;
	Accion *const accion = (Accion *const)datos;
	accion->activo = !accion->activo;
}


// Básicamente, oculta el botón 'Cambiar clave..."
//  si el proceso de administrador de claves esta abierto.
static void accion_esperar(GSubprocess *subproc,
			Accion *const accion)
{
	accion->activo = FALSE; // ref: accion_terminada
	g_subprocess_wait_async(subproc, NULL, accion_terminada, accion);
}


// * Dependencias:
// MATE : mate-system-tools
// GNOME: gnome-system-tools 
// XFCE : mate-system-tools
static void accion_de_notificacion(NotifyNotification *notify,
				char *action, gpointer dato)
{
	(void)notify;
	(void)action;
	// Ámbito #1
	GSubprocessLauncher *proc = g_subprocess_launcher_new(G_SUBPROCESS_FLAGS_NONE);

	if (NULL == proc) {
		g_warning(INFO "Failed to create instance of sub-process.\n");
		return;
	}

	// Ámbito #2
	GError *error = NULL;
	const Accion *const accion = (const Accion *const )dato; // opciones.accion
	const gchar *const admin = desktop_admin(accion);
	GSubprocess *subproc = g_subprocess_launcher_spawn(proc, &error, admin, NULL);

	if(NULL == subproc) {
		g_warning(INFO "Failed to execute sub-process %s", admin);
		goto error;
	}

	accion_esperar(subproc, (Accion *const)accion);

	g_clear_object(&subproc);
	g_clear_object(&proc);

	return;

error:
	g_error_free(error);
	g_clear_object(&proc);
}


static gboolean autenticar_notificar(const Opciones *const op)
{
	gchar *pass_cpy = g_strdup(op->clave);

	if (PAM_SUCCESS == pam_auth_user_pass(op->usuario, pass_cpy)) {
		// Con .time = NOTIFY_EXPIRES_NEVER, se muestra un cuadro de
		// diálogo con botones. 
		NotifyExtra extra = {
			.time = NOTIFY_EXPIRES_DEFAULT,
			.urgency = NOTIFY_URGENCY_CRITICAL,
			.callback = (op->accion.activo) ? accion_de_notificacion : NULL,
			.dato = (gpointer const)&op->accion
		};

		notify_wrap_show(op->titulo, op->mensaje, "gtk-dialog-warning", &extra);

		return TRUE;
	}

	return FALSE;
}



static gchar const *nombre_usuario_actual(void)
{
	const uid_t usuario_id = geteuid();
	const struct passwd *const datos = getpwuid(usuario_id);
	const gchar *const nombre = g_strdup(datos->pw_name);

	endpwent();
	return nombre;
}


static gpointer autenticar_notificar_thread(gpointer data)
{
	const Opciones *const op = (const Opciones *const)data;
	gboolean autenticado = FALSE;

	for (int i = 0; op->alarma.tiempo >= 0 && i <= op->alarma.repetir; ++i) {
		if (TRUE == (autenticado = autenticar_notificar(op)))
			sleep(op->alarma.tiempo);
	}

	// dirty-hack,
	// Antes de terminar, esperar a que la notificación se cierre.
	// Esto evita el desfase entre el servidor de notificaciones y el
	// programa.
	for (; autenticado && !notify_wrap_is_closed() ;)
		sleep(1); //seg

	if (autenticado)
		notify_wrap_end();

	g_main_loop_quit(main_loop);

	return NULL;
}


int main(int argc, char *argv[])
{
	const gchar *const nombre_usuario = nombre_usuario_actual();

	Opciones op = {
		.usuario  = nombre_usuario,
		.clave    = "alumno",
		.mensaje  = "Please, you must change the password.",
		.titulo   = nombre_usuario,
		.clave_usuario = FALSE,
		.accion   = (Accion){ .activo = TRUE, .nombre = NULL },
		.alarma   = (Alarma){ .tiempo = 0, .repetir = 1 } 
	};

	opciones (argc, argv, &op);

	GThread *thread = g_thread_new(NULL, autenticar_notificar_thread, &op);

	if (NULL == thread) {
		g_error(INFO "The thread start failed.");
		return EXIT_FAILURE;
	}

	main_loop = g_main_loop_new(NULL, FALSE);

	g_main_loop_run(main_loop);
}

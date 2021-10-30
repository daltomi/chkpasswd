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
#include "opciones.h"

static _Noreturn void ayuda(void);

static void opciones_alarma(Opciones *op);

// Soporta:
// -u para usuario
// -p para clave
// -m mensaje
// -t titulo del mensaje
// -a alarma
// -h ayuda
// -b sin botón "Cambiar clave..."
// -g para configurar usuarios y grupos
// -s usar el nombre de usuario para la clave

void opciones(int argc, char *argv[], Opciones *op)
{
	int opt = 0;

	while ((opt = getopt (argc, argv, "u:c:t:m:a:g:p:hbs")) != -1) {
		switch (opt) {
		case 'u':
			if (optarg != NULL)
				op->usuario = g_strdup(optarg);
		break;
		case 'p':
			if (optarg != NULL)
				op->clave = g_strdup(optarg);
		break;
		case 'm':
			if (optarg != NULL)
				op->mensaje = g_strdup(optarg);
		break;
		case 't':
			if (optarg != NULL)
				op->titulo = g_strdup(optarg);
		break;
		case 'a':
			if (optarg != NULL)
				opciones_alarma(op);
		break;
		case 'b':
			op->accion.activo = FALSE;
		break;
		case 's':
			op->clave_usuario = TRUE;
		break;
		case 'g':
			if (optarg != NULL)
			op->accion.nombre = g_strdup(optarg);
		break;
		default: // también implica -h
			ayuda();
		break;
		}
	}

	if (op->clave_usuario)
		op->clave = op->usuario;
}


void opciones_alarma (Opciones *op)
{
	const char *const delim = ":";

	char *tok = strtok(optarg, delim);
	if (tok != NULL)
		op->alarma.tiempo = atoi(tok);

	tok = strtok(NULL, delim);
	if (tok != NULL) {
		int valor = atoi(tok);
		op->alarma.repetir = (valor <= 0) ? 1 : valor;
	}
}


_Noreturn void ayuda(void)
{
	gchar const contenido [] = "The options are:\n"
	"-u to indicate the name of user. By default automatically detected\n"
	"-p to indicate the user password.\n"
	"-m to indicate the informational message. By default \"Please, you must change the password.\"\n"
	"-t to indicate the title of the message.\n"
	"-a to indicate the alarm, seconds:repeat\n"
	"-b not show action button, \"Change password ...\"\n"
	"-g name of the program to User Manager and Groups.\n"
	"-s use the username as password.\n"
	"-h show help..";

	g_printf("Help:\n%s\n", contenido);

	exit(EXIT_SUCCESS);
}

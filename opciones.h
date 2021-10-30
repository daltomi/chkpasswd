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

#pragma once

#include <unistd.h> //getopt
#include <stdlib.h> // exit
#include <string.h> // strtok
#include <pwd.h>    // getpwuid
#include <glib.h>
#include <glib/gprintf.h>


extern char *optarg;

// Simula alarm() con sleep()
typedef struct Alarma {
	int tiempo; // en segundos
	int repetir;
} Alarma;



typedef struct Accion {
	gboolean activo; // mostrar botón 'Cambiar clave...'
	gchar *nombre;
} Accion;


typedef struct Opciones {
	const gchar *usuario;
	const gchar *clave;
	const gchar *mensaje;
	const gchar *titulo;
	gboolean clave_usuario;
	Accion accion;
	Alarma alarma;
} Opciones;


void opciones(int argc, char *argv[], Opciones *op);


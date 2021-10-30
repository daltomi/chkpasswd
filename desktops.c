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

#include "desktops.h"

static Admin *admin_list (DesktopType type);
static gchar const *desktop_from_env ();

Admin *admin_list(DesktopType type)
{
	static Admin admins [NUM] = { 
		{ XFCE		, "/usr/bin/mate-users-admin" },
		{ MATE		, "/usr/bin/mate-users-admin" },
		{ GNOME		, "/usr/bin/users-admin"      },
		{ UKNOWN	, "/usr/bin/mate-users-admin" },
	};

	return &admins[type];
}


gchar const *desktop_admin(const Accion *const accion)
{
	if (NULL == accion->nombre)
		return desktop_from_env();

	return accion->nombre;
}


gchar const *desktop_from_env()
{
	const gchar *admin = NULL;

	gchar ** env = g_get_environ();

	const gchar *const current = g_environ_getenv(env, "XDG_CURRENT_DESKTOP");

	if (NULL == current) {
		/*
		* Algunos WM no asignan valor a XDG_CURRENT_DESKTOP.
		* Ej: jwm
		* */
		goto uknown;
	}

	if (!g_strcmp0(current, "MATE")) {
		admin = admin_list(MATE)->name;

	} else if (! g_strcmp0(current, "XFCE")) {
		admin = admin_list (XFCE)->name;

	} else if (! g_strcmp0 (current, "GNOME")) {
		admin = admin_list(GNOME)->name;

	} else {
uknown:
		admin = admin_list(UKNOWN)->name;
	}

	g_strfreev(env);

	g_assert(admin != NULL);

	return admin;
}

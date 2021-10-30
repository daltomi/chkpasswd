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

#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "version.h"

typedef struct pam_response pam_response;
typedef struct pam_message pam_message;


int pam_auth_user_pass (const char *const user, const char *const pass);


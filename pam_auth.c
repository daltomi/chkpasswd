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

#include "pam_auth.h"


static int conv(int num_msg, const pam_message **msg, pam_response **resp, void *appdata_ptr)
{
	(void)num_msg;
	(void)msg;
	pam_response *reply = malloc(sizeof(pam_response));

	reply->resp = (char*)appdata_ptr;
	reply->resp_retcode = 0;
	*resp = reply;
	return PAM_SUCCESS;
}



int pam_auth_user_pass (const char *const user, const char *const pass)
{
	struct pam_conv pamc = { conv, (void*)pass };
	pam_handle_t *pamh = NULL; 
	int retval = PAM_ABORT;

	if ((retval = pam_start("login", user, &pamc, &pamh)) == PAM_SUCCESS)
		retval = pam_authenticate(pamh, 0);

	g_message(INFO "pam authenticate: %s\n", pam_strerror(pamh, retval));
	pam_end(pamh, 0);
	return retval;
}


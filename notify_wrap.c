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

static int notify_wrap_init(void);
static void remove_ampersand(gchar const *str);
static void extra_init(const NotifyExtra *const extra);
static gboolean support_actions(void);

static NotifyNotification *notify_obj = NULL;

void notify_wrap_show(const gchar *summary,
		const gchar *body,
		const gchar *const icon,
		const NotifyExtra *extra)
{
	remove_ampersand(summary);
	remove_ampersand(body);

	if (notify_wrap_init() == 0) {
		g_error(INFO "Failed to start notify_wrap_init()\n");
		return;
	}

	if (notify_obj == NULL) {
		notify_obj = notify_notification_new(summary, body, icon);
		// TODO: a confirmar posible bug: ¿ es g_object_unref safe si
		// notify_obj ya es NULL ?
		if (NULL == notify_obj) {
			g_warning(INFO "Failed to create instance of notify_notification_new()\n");
			goto error_notification_new;
		}

	} else {
		notify_notification_update(notify_obj, summary, body, icon);
	}

	extra_init(extra);

	GError *err = NULL;
	const gboolean is_show = notify_notification_show(notify_obj, &err);

	if (FALSE == is_show) {
		g_warning(INFO "Failed to show notification\n");
		goto error_notification_show;
	}

	return;

error_notification_show:

	g_error_free(err);

error_notification_new:

	notify_wrap_end();
}



void extra_init(const NotifyExtra *const extra)
{
	if (extra == NULL)
		return;

	notify_notification_clear_actions(notify_obj);
	notify_notification_set_timeout(notify_obj, extra->time);
	notify_notification_set_urgency(notify_obj, extra->urgency);

	if (support_actions() && extra->callback != NULL) {
		notify_notification_add_action(notify_obj,
			"open",
			"Change password ...",
			extra->callback,
			extra->dato,
			NULL);
	}
}


gboolean support_actions(void)
{
	g_assert(notify_obj != NULL);

	GList *const caps = notify_get_server_caps();
	gboolean ret = FALSE;

	if (g_list_find_custom(caps, "actions", (GCompareFunc)strcmp))
		ret = TRUE;
	else
		g_warning(INFO "notify not support actions.");

	g_list_foreach(caps, (GFunc) g_free, NULL);
	g_list_free (caps);

	return ret;
}


// Sólo falla si se acabaron todas las instancias posibles.
int notify_wrap_init(void)
{
	if (!notify_is_initted())
		if (!notify_init("chkpasswd"))
			return 0;

	return 1;
}


void notify_wrap_end(void)
{
	g_object_unref(G_OBJECT(notify_obj));
	notify_obj = NULL;
	notify_uninit();
}


//This fix gtk &amp error
void remove_ampersand (const gchar *str)
{
	g_assert(NULL != str);

	gchar *find = NULL;

	while (NULL != (find = g_strrstr(str, "&")))
		*find = ' ';
}


gboolean notify_wrap_is_closed(void)
{
	g_assert(notify_obj != NULL);

	return -1 != notify_notification_get_closed_reason(notify_obj);
}


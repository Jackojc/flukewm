/* See LICENSE file for copyright and license details. */

#include <err.h>
#include <stdlib.h>
#include <xcb/xcb.h>

#include "util.h"

void
init_xcb(xcb_connection_t ** con)
{
	*con = xcb_connect(NULL, NULL);
	if (xcb_connection_has_error(*con))
		errx(1, "unable connect to the X server");
}

void
kill_xcb(xcb_connection_t ** con)
{
	if (*con)
		xcb_disconnect(*con);
}

void
get_screen(xcb_connection_t * con, xcb_screen_t ** scr)
{
	*scr = xcb_setup_roots_iterator(xcb_get_setup(con)).data;
	if (*scr == NULL)
		errx(1, "unable to retrieve screen informations");
}

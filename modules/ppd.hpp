/* See LICENSE file for copyright and license details. */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "randr.h"
#include "util.h"

xcb_connection_t *conn;
xcb_screen_t     *scrn;

void
usage(char *name)
{
	printf("usage: %s \n", name);
	exit(0);
}

int
main(int argc, char *argv[])
{
#ifdef __OpenBSD__
	if (pledge("stdio rpath unix", NULL) == -1)
		err(1, "pledge");
#endif
	xcb_randr_get_output_primary_reply_t *primary_output;

	if (argc > 1)
		usage(argv[0]);
	init_xcb(&conn);
	get_screen(conn, &scrn);

	primary_output = get_primary_output(conn);

	printf("%s\n", get_output_name(conn, primary_output->output));
	return 0;
}

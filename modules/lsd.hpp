/* See LICENSE file for copyright and license details. */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void
print_output(xcb_connection_t *conn, xcb_randr_output_t output)
{
	uint8_t* name = get_output_name(conn, output);
	if (get_output_connection(conn, output) == 0) {
		printf("%s\n", name);
	}
}

int
main(int argc, char *argv[])
{
	xcb_randr_get_output_info_reply_t *output_info;
	xcb_randr_get_crtc_info_reply_t   *output_crtc_info;
	int print_all = 0;
	if (argc > 1 && strcmp(argv[1], "-a") == 0)
		print_all = 1;

#ifdef __OpenBSD__
	if (pledge("stdio rpath unix", NULL) == -1)
		err(1, "pledge");
#endif
	if (argc > 1 && (argc == 2 && !print_all))
		usage(argv[0]);
	init_xcb(&conn);
	get_screen(conn, &scrn);

	xcb_randr_provider_t *providers;
	int p_len = get_providers(conn, scrn, &providers);

	for (int i = 0; i < p_len; i++) {
		xcb_randr_output_t *os;
		int o_len = get_outputs(conn, providers[i], &os);

		/* Print screens from providers */
		for (int j = 0; j < o_len; j++) {
			output_info = get_output_info(conn, os[j]);
			output_crtc_info = get_output_crtc_info(conn, output_info->crtc);
			/* Print screen only if in use */
			if (print_all || output_crtc_info != NULL) {
				print_output(conn, os[j]);
			}
		}
	}
	return 0;
}

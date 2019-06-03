/* See LICENSE file for copyright and license details. */

#ifndef RANDR_H__
#define RANDR_H__

#include <xcb/xcb.h>
#include <xcb/randr.h>
#include <stdlib.h>
#include <err.h>

int get_outputs(xcb_connection_t * conn, xcb_randr_provider_t provider, xcb_randr_output_t ** os);
int get_providers(xcb_connection_t * con, xcb_screen_t * scrn, xcb_randr_provider_t ** ps);

xcb_randr_get_output_info_reply_t *get_output_info(xcb_connection_t * conn, xcb_randr_output_t output);
uint8_t *get_output_name(xcb_connection_t * conn, xcb_randr_output_t output);
int get_output_connection(xcb_connection_t * conn, xcb_randr_output_t output);

xcb_randr_get_crtc_info_reply_t *get_output_crtc_info(xcb_connection_t * conn, xcb_randr_crtc_t crtc);
xcb_window_t get_focused_window(xcb_connection_t * conn);
xcb_get_geometry_reply_t *get_window_geometry(xcb_connection_t * conn, xcb_window_t window);
xcb_randr_get_output_primary_reply_t *get_primary_output(xcb_connection_t * conn);

#endif
/* end of include guard: WMUTILS_RANDR */

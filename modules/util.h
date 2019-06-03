/* See LICENSE file for copyright and license details. */

#ifndef UTIL_H__
#define UTIL_H__

void init_xcb (xcb_connection_t **);
void kill_xcb (xcb_connection_t **);

void get_screen(xcb_connection_t *, xcb_screen_t **);

#endif

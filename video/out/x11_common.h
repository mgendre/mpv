/*
 * This file is part of MPlayer.
 *
 * MPlayer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * MPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with MPlayer; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef MPLAYER_X11_COMMON_H
#define MPLAYER_X11_COMMON_H

#include <stdint.h>
#include <stdbool.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "common/common.h"

struct vo;
struct mp_log;

#define MAX_DISPLAYS 32 // ought to be enough for everyone

struct xrandr_display {
    struct mp_rect rc;
    double fps;
};

struct vo_x11_state {
    struct mp_log *log;
    Display *display;
    Window window;
    Window rootwin;
    int screen;
    int display_is_local;
    int ws_width;
    int ws_height;
    struct mp_rect screenrc;

    struct xrandr_display displays[MAX_DISPLAYS];
    int num_displays;

    int xrandr_event;

    bool screensaver_enabled;
    bool dpms_touched;
    double screensaver_time_last;

    XIM xim;
    XIC xic;
    bool no_autorepeat;

    GC f_gc;    // used to paint background
    GC vo_gc;   // used to paint video
    Colormap colormap;

    int wm_type;
    bool window_hidden;
    int fs;     // whether we assume the window is in fullscreen mode

    bool mouse_cursor_hidden;
    long orig_layer;

    // Current actual window position (updated on window move/resize events).
    struct mp_rect winrc;
    double current_display_fps;

    int pending_vo_events;

    // last non-fullscreen extends (updated on fullscreen or reinitialization)
    struct mp_rect nofsrc;

    /* Keep track of original video width/height to determine when to
     * resize window when reconfiguring. Resize window when video size
     * changes, but don't force window size changes as long as video size
     * stays the same (even if that size is different from the current
     * window size after the user modified the latter). */
    int old_dw, old_dh;
    /* Video size changed during fullscreen when we couldn't tell the new
     * size to the window manager. Must set window size when turning
     * fullscreen off. */
    bool size_changed_during_fs;
    bool pos_changed_during_fs;

    XComposeStatus compose_status;

    /* XShm stuff */
    int ShmCompletionEvent;
    /* Number of outstanding XShmPutImage requests */
    /* Decremented when ShmCompletionEvent is received */
    /* Increment it before XShmPutImage */
    int ShmCompletionWaitCount;

    /* drag and drop */
    Atom dnd_property;
    Atom dnd_requested_format;
    Window dnd_src_window;

    /* dragging the window */
    bool win_drag_button1_down;
};

int vo_x11_init(struct vo *vo);
void vo_x11_uninit(struct vo *vo);
int vo_x11_check_events(struct vo *vo);
bool vo_x11_screen_is_composited(struct vo *vo);
void vo_x11_config_vo_window(struct vo *vo, XVisualInfo *vis, int flags,
                             const char *classname);
void vo_x11_clear_background(struct vo *vo, const struct mp_rect *rc);
void vo_x11_clearwindow(struct vo *vo, Window vo_window);
int vo_x11_control(struct vo *vo, int *events, int request, void *arg);

#endif /* MPLAYER_X11_COMMON_H */

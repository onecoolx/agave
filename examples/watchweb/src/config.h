/* config.h - WatchWeb smartwatch browser
 * Copyright (C) 2026 Zhang Ji Peng <onecoolx@gmail.com>
 */
#ifndef _WATCHWEB_CONFIG_H_
#define _WATCHWEB_CONFIG_H_

/* Display */
#define SCREEN_WIDTH    480
#define SCREEN_HEIGHT   480
#define SCREEN_RADIUS   240

/* Content area - full screen (the round display clips corners itself) */
#define CONTENT_INSET   0
#define CONTENT_TOP     0
#define CONTENT_WIDTH   SCREEN_WIDTH
#define CONTENT_HEIGHT  SCREEN_HEIGHT

/* Tile buffer. Width has horizontal margin for stable rendering; the layout
   viewport is pinned to the screen width via minimum_layout_size so pages lay
   out as a 480px mobile browser. Height is 3x for smooth vertical scrolling. */
#define TILE_BUF_W      (CONTENT_WIDTH * 2)
#define TILE_BUF_H      (CONTENT_HEIGHT * 3)

/* UI metrics */
#define NAV_BTN_SIZE    56
#define PROGRESS_WIDTH  4
/* Floating toggle icon at bottom-center */
#define FAB_SIZE        40
#define FAB_MARGIN      8
/* Bottom navigation panel (slides up from bottom) */
#define NAV_BAR_H       88

/* Zoom (1.0 = render the 480px-wide layout 1:1; user may zoom in for detail) */
#define DEFAULT_ZOOM    1.0f
#define MIN_ZOOM        0.5f
#define MAX_ZOOM        4.0f

/* Gesture */
#define SWIPE_THRESHOLD 40
#define SCROLL_DECEL    0.95f

#endif /* _WATCHWEB_CONFIG_H_ */

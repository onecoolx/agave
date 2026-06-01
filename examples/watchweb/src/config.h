/* config.h - WatchWeb smartwatch browser
 * Copyright (C) 2026 Zhang Ji Peng <onecoolx@gmail.com>
 */
#ifndef _WATCHWEB_CONFIG_H_
#define _WATCHWEB_CONFIG_H_

/* Display */
#define SCREEN_WIDTH    480
#define SCREEN_HEIGHT   480
#define SCREEN_RADIUS   240

/* Content area (inscribed usable rect in circle) */
#define CONTENT_INSET   40
#define CONTENT_TOP     56
#define CONTENT_WIDTH   (SCREEN_WIDTH - 2 * CONTENT_INSET)
#define CONTENT_HEIGHT  (SCREEN_HEIGHT - CONTENT_TOP - CONTENT_INSET - 50)

/* Tile buffer (2x width, 3x height for smooth scrolling) */
#define TILE_BUF_W      (CONTENT_WIDTH * 2)
#define TILE_BUF_H      (CONTENT_HEIGHT * 3)

/* UI metrics */
#define ADDR_BAR_H      40
#define ADDR_BAR_Y      6
#define ADDR_BAR_X      70
#define ADDR_BAR_W      (SCREEN_WIDTH - 2 * ADDR_BAR_X)
#define NAV_BTN_SIZE    36
#define PROGRESS_WIDTH  3

/* Zoom */
#define DEFAULT_ZOOM    1.5f
#define MIN_ZOOM        0.5f
#define MAX_ZOOM        4.0f

/* Gesture */
#define SWIPE_THRESHOLD 40
#define SCROLL_DECEL    0.95f

#endif /* _WATCHWEB_CONFIG_H_ */

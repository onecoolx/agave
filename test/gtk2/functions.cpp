/* functions.cpp - Agave application 
 * 
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "main_win.h"

extern GtkWidget* getMainWindow();
extern MaCrossView* OpenWindow(const char*, int,int);
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void update_rect(void *p, int x, int y, int w, int h)
{
	MainWindow* main = (MainWindow*)p;
	main->update(x, y, w, h);
}

void set_title(void *p, const char* text)
{
	MainWindow* main = (MainWindow*)p;
	main->setWindowTitle(text);
}

void update_window(void *p)
{
}

void change_cursor(int c)
{
    GdkDisplay * disp = gdk_display_get_default();
    GdkScreen * screen = gdk_screen_get_default();
    GdkWindow * win = gdk_screen_get_root_window(screen);
    GdkCursor * cursor = NULL;

	switch (c) {
	case 1:
        cursor = gdk_cursor_new_for_display(disp, GDK_CROSSHAIR);
        gdk_window_set_cursor(win, cursor);
		break;
	case 2:
        cursor = gdk_cursor_new_for_display(disp, GDK_HAND2);
        gdk_window_set_cursor(win, cursor);
		break;
	case 3:
        cursor = gdk_cursor_new_for_display(disp, GDK_FLEUR);
        gdk_window_set_cursor(win, cursor);
		break;
	case 4:
        cursor = gdk_cursor_new_for_display(disp, GDK_PENCIL);
        gdk_window_set_cursor(win, cursor);
		break;
	case 5:
        cursor = gdk_cursor_new_for_display(disp, GDK_WATCH);
        gdk_window_set_cursor(win, cursor);
		break;
	case 6:
        cursor = gdk_cursor_new_for_display(disp, GDK_WATCH);
        gdk_window_set_cursor(win, cursor);
		break;
	default:
        cursor = gdk_cursor_new_for_display(disp, GDK_LEFT_PTR);
        gdk_window_set_cursor(win, cursor);
		break;
	}
}

MaCrossView* window_open(const char* url, int x, int y, int w, int h)
{
	return OpenWindow(url, w, h);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

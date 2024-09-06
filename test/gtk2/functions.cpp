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
#if 0
	QMainWindow* window = getMainWindow();
	switch (c) {
	case 1:
    	window->setCursor(Qt::CrossCursor);
		break;
	case 2:
    	window->setCursor(Qt::PointingHandCursor);
		break;
	case 3:
    	window->setCursor(Qt::SizeAllCursor);
		break;
	case 4:
    	window->setCursor(Qt::IBeamCursor);
		break;
	case 5:
    	window->setCursor(Qt::WaitCursor);
		break;
	case 6:
    	window->setCursor(Qt::WhatsThisCursor);
		break;
	default:
    	window->setCursor(Qt::ArrowCursor);
		break;
	}
#endif
}

MaCrossView* window_open(const char* url, int x, int y, int w, int h)
{
	return OpenWindow(url, w, h);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

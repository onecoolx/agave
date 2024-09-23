/* webview_gtk2.h - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _WEBVIEW_GTK2_H_
#define _WEBVIEW_GTK2_H_

#include <gtk/gtk.h>

class WebView;
class MainWindowImpl;

class WebViewPlat
{
public:
	WebViewPlat(WebView* view);
	~WebViewPlat();

	bool CreateStoreBuffer(int width, int height);
	void DestroyStoreBuffer(void);

	void Paint(MainWindowImpl* window, int sx, int sy, int x, int y, int w, int h);

	unsigned char* GetBuffer(int* pitch);

	ps_context* getContext(void);
	static bool haveMemory(void);
private:
	WebView* m_view;
    GdkPixbuf*  m_buf;
	ps_context* m_context;
	ps_canvas* m_canvas;
};

#endif/*_WEBVIEW_QT4_H_*/

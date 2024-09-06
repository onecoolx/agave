/* main.cpp - Agave application 
 * 
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _AGAVE_GTK2_WIN_H_
#define _AGAVE_GTK2_WIN_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "macross.h"
#include "interface.h"

class MainWindow
{
public:
	MainWindow(int w, int h)
        : view(NULL)
        , window(NULL)
        , drawarea(NULL)
        , pixbuf(NULL)
        , width(w)
        , height(h)
	{
        pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, width, height);
        gchar* buf = (gchar*)gdk_pixbuf_get_pixels(pixbuf);
        gint stride = gdk_pixbuf_get_rowstride(pixbuf);
		gdk_pixbuf_fill(pixbuf, 0xFFFFFFFF);

        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        drawarea = gtk_drawing_area_new();
        gtk_window_set_default_size(GTK_WINDOW(window), width, height);

        g_signal_connect (G_OBJECT(window), "destroy", G_CALLBACK (MainWindow::destroy), this);
        g_signal_connect (G_OBJECT(drawarea), "expose_event", G_CALLBACK (MainWindow::expose), this);
        g_signal_connect (G_OBJECT(window), "size_allocate", G_CALLBACK (MainWindow::sizeChange), this);
#if 0
        g_signal_connect (G_OBJECT(window), "key_press_event", G_CALLBACK (key_press), NULL);
        g_signal_connect (G_OBJECT(window), "key_release_event", G_CALLBACK (key_release), NULL);

        gtk_widget_add_events (GTK_WIDGET(window), GDK_BUTTON_PRESS_MASK);
        g_signal_connect (G_OBJECT(window), "button_press_event", G_CALLBACK (mouse_button_press), NULL);
        gtk_widget_add_events (GTK_WIDGET(window), GDK_BUTTON_RELEASE_MASK);
        g_signal_connect (G_OBJECT(window), "button_release_event", G_CALLBACK (mouse_button_release), NULL);
        gtk_widget_add_events (GTK_WIDGET(window), GDK_POINTER_MOTION_MASK);
        g_signal_connect (G_OBJECT(window), "motion_notify_event", G_CALLBACK (mouse_motion_notify), NULL);
#endif

        gtk_container_add (GTK_CONTAINER (window), drawarea);

		view = on_init((unsigned char*)buf, width, height, stride, this);		
	}

	virtual ~MainWindow()
	{
	    on_term(view);
        g_object_unref(pixbuf);
        gtk_main_quit();
	}

    void setWindowTitle(const char * title)
    {
        gtk_window_set_title(GTK_WINDOW(window), title);
    }

	void loadUrl(const char* url)
	{
		load_url(view, url);
	}

    void show(void)
    {
        gtk_widget_show(drawarea);
        gtk_widget_show(window);
    }

    void update(int x, int y, int w, int h)
    {
    }

    void resize(int w, int h)
    {
        gtk_widget_set_size_request(window, w, h);
    }

	MaCrossView * getView(void) 
	{
		return view;
	}

    GtkWidget* getWindow(void)
    {
        return window;
    }

protected:
    static gboolean expose(GtkWidget *widget, GdkEventExpose *event, gpointer data);
    static void sizeChange(GtkWidget* widget, GtkAllocation* allocation, gpointer data);
#if 0
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
    void focusInEvent(QFocusEvent * event);
    void focusOutEvent(QFocusEvent * event);
    void contextMenuEvent(QContextMenuEvent * event);
#endif
    static void destroy(GtkWidget *widget, gpointer data);

    void onDraw(int x, int y, int w, int h)
    {
	    on_draw(view, x, y, w, h);
        gdk_draw_pixbuf(drawarea->window, drawarea->style->white_gc, pixbuf, 0, 0, 0, 0, width, height, GDK_RGB_DITHER_NONE,0,0);
    }

    void onSize(int w, int h)
    {
        g_object_unref(pixbuf);
	    if (w < 1) w = 1;
	    if (h < 1) h = 1;
        width = w;
        height = h;
        pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, w, h);
        gchar* buf = (gchar*)gdk_pixbuf_get_pixels(pixbuf);
        gint stride = gdk_pixbuf_get_rowstride(pixbuf);
        gdk_pixbuf_fill(pixbuf, 0xFFFFFFFF);
	    on_size(view, (unsigned char*)buf, w, h, stride);
    }

private:
	MaCrossView* view;
    GtkWidget *window;
    GtkWidget *drawarea;
    GdkPixbuf* pixbuf;
    int width;
    int height;
};

#endif /*_AGAVE_GTK2_WIN_H_*/

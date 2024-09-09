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
        g_signal_connect (G_OBJECT(window), "key_press_event", G_CALLBACK (MainWindow::keyPress), this);
        g_signal_connect (G_OBJECT(window), "key_release_event", G_CALLBACK (MainWindow::keyRelease), this);

        gtk_widget_add_events (GTK_WIDGET(window), GDK_BUTTON_PRESS_MASK);
        g_signal_connect (G_OBJECT(window), "button_press_event", G_CALLBACK (MainWindow::mouseButtonPress), this);
        gtk_widget_add_events (GTK_WIDGET(window), GDK_BUTTON_RELEASE_MASK);
        g_signal_connect (G_OBJECT(window), "button_release_event", G_CALLBACK (MainWindow::mouseButtonRelease), this);
        gtk_widget_add_events (GTK_WIDGET(window), GDK_POINTER_MOTION_MASK);
        g_signal_connect (G_OBJECT(window), "motion_notify_event", G_CALLBACK (MainWindow::mouseMotionNotify), this);

        gtk_widget_add_events (GTK_WIDGET(window), GDK_FOCUS_CHANGE_MASK);
        g_signal_connect (G_OBJECT(window), "focus-in-event", G_CALLBACK (MainWindow::focusInEvent), this);
        g_signal_connect (G_OBJECT(window), "focus-out-event", G_CALLBACK (MainWindow::focusOutEvent), this);

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
        gtk_widget_queue_draw_area(GTK_WIDGET(drawarea), x, y, w, h);
    }

    void update(void)
    {
        gtk_widget_queue_draw(GTK_WIDGET(drawarea));
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
    static gboolean keyPress(GtkWidget *widget, GdkEventKey *event, gpointer data);
    static gboolean keyRelease(GtkWidget *widget, GdkEventKey *event, gpointer data);

    static gboolean mouseButtonPress(GtkWidget *widget, GdkEventButton *event, gpointer data);
    static gboolean mouseButtonRelease(GtkWidget *widget, GdkEventButton *event, gpointer data);
    static gboolean mouseMotionNotify(GtkWidget *widget, GdkEventMotion *event, gpointer data);

    static gboolean focusInEvent(GtkWidget *widget, GdkEventFocus *event, gpointer data);
    static gboolean focusOutEvent(GtkWidget *widget, GdkEventFocus *event, gpointer data);

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

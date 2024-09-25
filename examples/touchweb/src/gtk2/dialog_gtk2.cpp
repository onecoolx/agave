/* dialog_gtk2.cpp - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "../config.h"
#include "../dialog.h"

#include <picasso/picasso.h>

#include "dialog_gtk2.h"
#include "mainwindow_gtk2.h"

DialogImpl::DialogImpl(Dialog* d)
    : m_dlg(d), m_gtkwindow(NULL), m_drawarea(NULL)
    , m_buf(NULL), m_gc(NULL), m_canvas(NULL)
    , m_x(0), m_y(0), m_w(0), m_h(0)
    , m_inst(NULL)
{
}

DialogImpl::~DialogImpl()
{
    if (m_gc) {
        ps_context_unref(m_gc);
        m_gc = NULL;
    }

    if (m_canvas) {
        ps_canvas_unref(m_canvas);
        m_canvas = NULL;
    }

    if (m_buf) {
        g_object_unref(m_buf);
        m_buf = NULL;
    }
}

void DialogImpl::OnUpdate(int x, int y, int w, int h)
{
    gtk_widget_queue_draw_area(GTK_WIDGET(m_drawarea), x, y, w, h);
}

void DialogImpl::OnPaint(int x, int y, int w, int h)
{
    Rect rc(x, y, w, h);
    m_dlg->Paint(m_gc, &rc);
}

void DialogImpl::OnCreate(int x, int y, int w, int h)
{
    m_dlg->OnCreate(0, x, y, w, h);
}

void DialogImpl::OnChar(unsigned int c)
{
    m_dlg->CharInput(c);
}

void DialogImpl::OnMouse(int t, unsigned b, int x, int y)
{
    MouseEvent evt(t, b, x, y);
    m_dlg->SendMouseEvent(&evt);
}

#if 0
void DialogImpl::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QRect rect = event->rect();
    OnPaint(rect.x(), rect.y(), rect.width(), rect.height());
    painter.drawImage(rect, *(m_data->m_img), rect);
}

void DialogImpl::mousePressEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton) {
        OnMouse(1, 1, event->x(), event->y());
    }
}

void DialogImpl::mouseReleaseEvent(QMouseEvent* event)
{
    OnMouse(0, 1, event->x(), event->y());
}

void DialogImpl::mouseMoveEvent(QMouseEvent* event)
{
    OnMouse(2, 0, event->x(), event->y());
}

void DialogImpl::keyPressEvent(QKeyEvent* event)
{
    OnChar(*(event->text().utf16()));
}
#endif

void DialogImpl::EndModal(int code)
{
    gtk_dialog_response(GTK_DIALOG(m_gtkwindow), code);
}

int DialogImpl::DoModal(void)
{
    return gtk_dialog_run(GTK_DIALOG(m_gtkwindow));
}

void DialogImpl::Create(const MainWindowImpl* main, int x, int y, int w, int h)
{
    m_inst = main->getInstance();
    m_x = x;
    m_y = y;
    m_w = w;
    m_h = h;

    m_gtkwindow = gtk_dialog_new();
    m_drawarea = gtk_drawing_area_new();
    gtk_window_set_default_size(GTK_WINDOW(m_gtkwindow), w, h);

    g_signal_connect (G_OBJECT(m_drawarea), "expose_event", G_CALLBACK (DialogImpl::expose), this);
    g_signal_connect (G_OBJECT(m_gtkwindow), "key_press_event", G_CALLBACK (DialogImpl::keyPress), this);

    gtk_widget_add_events (GTK_WIDGET(m_gtkwindow), GDK_BUTTON_PRESS_MASK);
    g_signal_connect (G_OBJECT(m_gtkwindow), "button_press_event", G_CALLBACK (DialogImpl::mouseButtonPress), this);
    gtk_widget_add_events (GTK_WIDGET(m_gtkwindow), GDK_BUTTON_RELEASE_MASK);
    g_signal_connect (G_OBJECT(m_gtkwindow), "button_release_event", G_CALLBACK (DialogImpl::mouseButtonRelease), this);
    gtk_widget_add_events (GTK_WIDGET(m_gtkwindow), GDK_POINTER_MOTION_MASK);
    g_signal_connect (G_OBJECT(m_gtkwindow), "motion_notify_event", G_CALLBACK (DialogImpl::mouseMotionNotify), this);

    OnCreate(0, 0, w, h);

    m_buf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, w, h);
    gchar* buf = (gchar*)gdk_pixbuf_get_pixels(m_buf);
    gint stride = gdk_pixbuf_get_rowstride(m_buf);
    gdk_pixbuf_fill(m_buf, 0xFFFFFFFF);

    m_canvas = ps_canvas_create_with_data((ps_byte*)buf, COLOR_FORMAT_RGB, w, h, stride);
    m_gc = ps_context_create(m_canvas, 0);

    gtk_container_add (GTK_CONTAINER (m_gtkwindow), m_drawarea);
}

gboolean DialogImpl::expose(GtkWidget* widget, GdkEventExpose* event, gpointer data)
{
}

gboolean DialogImpl::mouseButtonPress(GtkWidget* widget, GdkEventButton* event, gpointer data)
{
}

gboolean DialogImpl::mouseButtonRelease(GtkWidget* widget, GdkEventButton* event, gpointer data)
{
}

gboolean DialogImpl::mouseMotionNotify(GtkWidget* widget, GdkEventMotion* event, gpointer data)
{
}

gboolean DialogImpl::keyPress(GtkWidget* widget, GdkEventKey* event, gpointer data)
{
}

/* mainwindow_gtk2.cpp - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "../config.h"
#include "../unicode.h"
#include "../event.h"
#include "../mainwindow.h"

#include "mainwindow_gtk2.h"
#include "application_gtk2.h"

MainWindowImpl::MainWindowImpl(MainWindow* main)
    : m_window(main)
    , m_gtkwindow(NULL)
    , m_drawarea(NULL)
    , m_buf(NULL)
    , m_gc(NULL)
    , m_canvas(NULL)
    , m_view(NULL)
    , m_dirty(0, 0, 0, 0)
    , m_vx(0)
    , m_vy(0)
    , m_cx(0)
    , m_cy(0)
    , m_sx(0)
    , m_sy(0)
    , m_hInst(NULL)
{
}

MainWindowImpl::~MainWindowImpl()
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

void* MainWindowImpl::getInstance(void) const
{
    return m_hInst;
}

gboolean MainWindowImpl::expose(GtkWidget* widget, GdkEventExpose* event, gpointer data)
{
    MainWindowImpl* mainWindow = (MainWindowImpl*)data;
    GdkRectangle* r = &event->area;
    mainWindow->OnPaint(mainWindow->m_drawarea, r->x, r->y, r->width, r->height);
    gdk_draw_pixbuf(mainWindow->m_drawarea->window,
                    mainWindow->m_drawarea->style->white_gc, mainWindow->m_buf, r->x, r->y, r->x, r->y, r->width, r->height, GDK_RGB_DITHER_NONE, 0, 0);
    return FALSE;
}

gboolean MainWindowImpl::mouseButtonPress(GtkWidget* widget, GdkEventButton* event, gpointer data)
{
    MainWindowImpl* mainWindow = (MainWindowImpl*)data;
	if (event->button == 1) {
        mainWindow->OnMouse(1, 1, event->x, event->y);
    }
    return FALSE;
}

gboolean MainWindowImpl::mouseButtonRelease(GtkWidget* widget, GdkEventButton* event, gpointer data)
{
    MainWindowImpl* mainWindow = (MainWindowImpl*)data;
    mainWindow->OnMouse(0, 1, event->x, event->y);
    return FALSE;
}

gboolean MainWindowImpl::mouseMotionNotify(GtkWidget* widget, GdkEventMotion* event, gpointer data)
{
    MainWindowImpl* mainWindow = (MainWindowImpl*)data;
    mainWindow->OnMouse(2, 0, event->x, event->y);
    return FALSE;
}

gboolean MainWindowImpl::keyPress(GtkWidget* widget, GdkEventKey* event, gpointer data)
{
    MainWindowImpl* mainWindow = (MainWindowImpl*)data;
    mainWindow->OnKey(1, get_virtual_key(event->keyval));
    ustring str = Unicode::ConvertUTF8ToUTF16(std::string(event->string));
    mainWindow->OnChar(str[0]);
    return FALSE;
}

gboolean MainWindowImpl::keyRelease(GtkWidget* widget, GdkEventKey* event, gpointer data)
{
    MainWindowImpl* mainWindow = (MainWindowImpl*)data;
    mainWindow->OnKey(0, get_virtual_key(event->keyval));
    return FALSE;
}

void MainWindowImpl::destroy(GtkWidget* widget, gpointer data)
{
    MainWindowImpl* mainWindow = (MainWindowImpl*)data;
    mainWindow->m_window->Destroy();
}

bool MainWindowImpl::Create(void* hInstance, const char* title, int x, int y, int w, int h)
{
    m_hInst = hInstance;

    m_gtkwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    m_drawarea = gtk_drawing_area_new();
    gtk_window_set_position(GTK_WINDOW(m_gtkwindow), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(m_gtkwindow), w, h);

    gtk_window_set_title(GTK_WINDOW(m_gtkwindow), title);

    g_signal_connect (G_OBJECT(m_gtkwindow), "destroy", G_CALLBACK (MainWindowImpl::destroy), this);
    g_signal_connect (G_OBJECT(m_drawarea), "expose_event", G_CALLBACK (MainWindowImpl::expose), this);
    g_signal_connect (G_OBJECT(m_gtkwindow), "key_press_event", G_CALLBACK (MainWindowImpl::keyPress), this);
    g_signal_connect (G_OBJECT(m_gtkwindow), "key_release_event", G_CALLBACK (MainWindowImpl::keyRelease), this);

    gtk_widget_add_events (GTK_WIDGET(m_gtkwindow), GDK_BUTTON_PRESS_MASK);
    g_signal_connect (G_OBJECT(m_gtkwindow), "button_press_event", G_CALLBACK (MainWindowImpl::mouseButtonPress), this);
    gtk_widget_add_events (GTK_WIDGET(m_gtkwindow), GDK_BUTTON_RELEASE_MASK);
    g_signal_connect (G_OBJECT(m_gtkwindow), "button_release_event", G_CALLBACK (MainWindowImpl::mouseButtonRelease), this);
    gtk_widget_add_events (GTK_WIDGET(m_gtkwindow), GDK_POINTER_MOTION_MASK);
    g_signal_connect (G_OBJECT(m_gtkwindow), "motion_notify_event", G_CALLBACK (MainWindowImpl::mouseMotionNotify), this);

    OnCreate(x, y, w, h);

    m_buf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, w, h);
    gchar* buf = (gchar*)gdk_pixbuf_get_pixels(m_buf);
    gint stride = gdk_pixbuf_get_rowstride(m_buf);
    gdk_pixbuf_fill(m_buf, 0xFFFFFFFF);

    m_canvas = ps_canvas_create_with_data((ps_byte*)buf, COLOR_FORMAT_RGB, w, h, stride);
    m_gc = ps_context_create(m_canvas, 0);

    gtk_container_add (GTK_CONTAINER (m_gtkwindow), m_drawarea);
    gtk_widget_show(m_drawarea);
    gtk_widget_show(m_gtkwindow);
    return true;
}

void MainWindowImpl::Destroy(void)
{
    OnDestroy();
    gtk_main_quit();
}

void MainWindowImpl::OnUpdate(int x, int y, int w, int h)
{
    gtk_widget_queue_draw_area(GTK_WIDGET(m_drawarea), x, y, w, h);
}

void MainWindowImpl::OnCreate(int x, int y, int w, int h)
{
    m_window->OnCreate(0, x, y, w, h);
}

void MainWindowImpl::OnDestroy(void)
{
    m_window->OnDestroy();
}

void MainWindowImpl::OnMouse(int t, unsigned b, int x, int y)
{
    MouseEvent evt(t, b, x, y);
    m_window->SendMouseEvent(&evt);
}

void MainWindowImpl::OnKey(int t, unsigned vk)
{
    KeyEvent evt(t, vk);
    m_window->SendKeyEvent(&evt);
}

void MainWindowImpl::OnPaint(GtkWidget* p, int x, int y, int w, int h)
{
    Rect rc(x, y, w, h);
    m_window->Paint(m_gc, &rc);
    paintProcess(m_gc);
}

void MainWindowImpl::OnChar(unsigned int c)
{
    m_window->CharInput(c);
}

void MainWindowImpl::paintProcess(ps_context* gc)
{
    if (m_view && !m_dirty.isEmpty()) {
        cairo_t* cr = gdk_cairo_create(m_drawarea->window);

        int x = m_dirty.x + m_vx;
        int y = m_dirty.y + m_vy;
        int w = m_dirty.w;
        int h = m_dirty.h;
        GdkRectangle cliprect = {m_vx, m_vy, m_cx, m_cy};
        //clip topmost childs area.
        std::list<Widget*> tops = m_window->getChilds(true);
        cairo_save(cr);
        if (!tops.empty()) {
            GdkRegion* clip = gdk_region_rectangle(&cliprect);
            GdkRegion* clipouts = gdk_region_new();
            for (std::list<Widget*>::iterator it = tops.begin(); it != tops.end(); it++) {
                Rect erc(x, y, w, h);
                Rect rc((*it)->boundRect().x, (*it)->boundRect().y, (*it)->boundRect().w, (*it)->boundRect().h);
                if ((*it)->isVisible() && erc.intersect(rc)) {
                    //clip area
                    GdkRectangle cxr = {erc.x, erc.y, erc.w, erc.h};
                    gdk_region_union_with_rect(clipouts, &cxr);
                }
            }
            gdk_region_subtract(clip, clipouts);
            gdk_cairo_region(cr, clip);
            cairo_clip(cr);
            gdk_region_destroy(clip);
            gdk_region_destroy(clipouts);
        }

        gdk_cairo_set_source_pixbuf (cr, m_view, -x, -y);
        cairo_rectangle(cr, m_dirty.x - m_sx, m_dirty.y - m_sy, w, h);
        cairo_paint (cr);

        cairo_restore(cr);

        if (!tops.empty()) {
            for (std::list<Widget*>::iterator it = tops.begin(); it != tops.end(); it++) {
                Rect erc(x, y, w, h);
                Rect rc((*it)->boundRect().x, (*it)->boundRect().y, (*it)->boundRect().w, (*it)->boundRect().h);
                if ((*it)->isVisible() && erc.intersect(rc)) {
                    //draw top window
                    gdk_cairo_set_source_pixbuf (cr, m_buf, -erc.x, -erc.y);
                    cairo_rectangle(cr, erc.x, erc.y, erc.w, erc.h);
                    cairo_paint (cr);
                }
            }
        }

        cairo_reset_clip(cr);
        cairo_destroy(cr);

        m_view = 0;
        m_dirty = Rect(0, 0, 0, 0);
        m_vx = 0;
        m_vy = 0;

    }
}

void MainWindowImpl::DrawImage(const GdkPixbuf* buf, int vx, int vy, int cx, int cy, int sx, int sy, int x, int y, int w, int h)
{
    m_view = buf;
#if 0
    m_dirty = Rect(x, y, w, h);
#else
    //FIXME:temp for draw whole area for main page. is it slow
    m_dirty = Rect(0, 0, cx, cy);
#endif
    m_vx = vx;
    m_vy = vy;
    m_cx = cx;
    m_cy = cy;
    m_sx = sx;
    m_sy = sy;
}

int MainWindowImpl::SysWidth(void)
{
    return DEFAULT_WIDTH;
}

int MainWindowImpl::SysHeight(void)
{
    return DEFAULT_HEIGHT;
}

bool MainWindowImpl::getChooseFile(uchar_t* name, unsigned int len)
{
    return false;
}

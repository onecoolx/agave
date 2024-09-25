/* mainwindow_gtk2.h - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _MAINWINDOW_GTK2_H_
#define _MAINWINDOW_GTK2_H_

#include <picasso/picasso.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

class MainWindowImpl
{
public:
    MainWindowImpl(MainWindow* main);
    ~MainWindowImpl();

    bool Create(void* hInstance, const char* title, int x, int y, int w, int h);
    void Destroy(void);

    void* getInstance(void) const;
    //event handle
    void OnPaint(GtkWidget* p, int x, int y, int w, int h);
    void OnCreate(int x, int y, int w, int h);
    void OnUpdate(int x, int y, int w, int h);
    void OnChar(unsigned int c);
    void OnDestroy(void);

    void OnMouse(int type, unsigned btn, int x, int y);
    void OnKey(int type, unsigned vk);

    void DrawImage(const GdkPixbuf* buf, int vx, int vy, int cx, int cy, int sx, int sy, int x, int y, int w, int h);
    int SysWidth(void);
    int SysHeight(void);
public:
    bool getChooseFile(uchar_t* name, unsigned int len);
protected:
    static gboolean expose(GtkWidget* widget, GdkEventExpose* event, gpointer data);

    static gboolean mouseButtonPress(GtkWidget* widget, GdkEventButton* event, gpointer data);
    static gboolean mouseButtonRelease(GtkWidget* widget, GdkEventButton* event, gpointer data);
    static gboolean mouseMotionNotify(GtkWidget* widget, GdkEventMotion* event, gpointer data);

    static gboolean keyPress(GtkWidget* widget, GdkEventKey* event, gpointer data);
    static gboolean keyRelease(GtkWidget* widget, GdkEventKey* event, gpointer data);

    static void destroy(GtkWidget* widget, gpointer data);
private:
    void paintProcess(ps_context* gc);

    MainWindow* m_window;
    GtkWidget* m_gtkwindow;
    GtkWidget* m_drawarea;
    GdkPixbuf* m_buf;
    ps_context* m_gc;
    ps_canvas* m_canvas;
    const GdkPixbuf* m_view;
    Rect m_dirty;
    int m_vx;
    int m_vy;
    int m_cx;
    int m_cy;
    int m_sx;
    int m_sy;
    void* m_hInst;
};

#endif/*_MAINWINDOW_GTK2_H_*/

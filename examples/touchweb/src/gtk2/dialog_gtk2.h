/* dialog_gtk2.h - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _DIALOG_GTK2_H_
#define _DIALOG_GTK2_H_

#include <gtk/gtk.h>
#include <picasso/picasso.h>

class MainWindowImpl;

class DialogImpl
{
public:
    DialogImpl(Dialog* d);
    ~DialogImpl();

    int DoModal(void);
    void EndModal(int code);

    void Create(const MainWindowImpl* main, int x, int y, int w, int h);

    void OnPaint(int x, int y, int w, int h);
    void OnCreate(int x, int y, int w, int h);
    void OnUpdate(int x, int y, int w, int h);
    void OnChar(unsigned int c);
    void OnMouse(int type, unsigned btn, int x, int y);
protected:
    static gboolean expose(GtkWidget* widget, GdkEventExpose* event, gpointer data);

    static gboolean mouseButtonPress(GtkWidget* widget, GdkEventButton* event, gpointer data);
    static gboolean mouseButtonRelease(GtkWidget* widget, GdkEventButton* event, gpointer data);
    static gboolean mouseMotionNotify(GtkWidget* widget, GdkEventMotion* event, gpointer data);

    static gboolean keyPress(GtkWidget* widget, GdkEventKey* event, gpointer data);
private:
    Dialog* m_dlg;
    GtkWidget* m_gtkwindow;
    GtkWidget* m_drawarea;
    GdkPixbuf* m_buf;
    ps_context* m_gc;
    ps_canvas* m_canvas;
    int m_x;
    int m_y;
    int m_w;
    int m_h;
    void* m_inst;
};

#endif/*_DIALOG_GTK2_H_*/

/* application_gtk2.h - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _APPLICATION_GTK2_H_
#define _APPLICATION_GTK2_H_

#include <string>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

class GtkLoop
{
public:
    GtkLoop()
        : tid(0), eid(0)
    {
        eid = g_idle_add(GtkLoop::IdleCb, this);
    }

    virtual ~GtkLoop()
    {
        g_source_remove(tid);
        g_source_remove(eid);
    }

    void start(int ms)
    {
        tid = g_timeout_add(ms, GtkLoop::TimerCb, this);
    }

    static gboolean TimerCb(gpointer data)
    {
        GtkLoop* t = (GtkLoop*)data;
        t->timerEvent();
        return TRUE;
    }
    static gboolean IdleCb(gpointer data)
    {
        GtkLoop* t = (GtkLoop*)data;
        t->idleEvent();
        return TRUE;
    }
protected:
    virtual void timerEvent(void) = 0;
    virtual void idleEvent(void) = 0;
private:
    gint tid;
    gint eid;
};

class Application;
class ApplicationImpl : public GtkLoop
{
public:
    ApplicationImpl(Application*);
    virtual ~ApplicationImpl();

    int screen_width(void) const;
    int screen_height(void) const;

    void init(void);
    int run_loop(void);

    ps_color_format color_format(void) const { return m_format; }
    int bits_pixel(void) const { return m_cbit; }
    int bytes_pixel(void) const { return m_cbyte; }
    int today(void) const;
    bool macAddress(std::string&);

    static unsigned long tickCount(void);
protected:
    void timerEvent(void);
    void idleEvent(void);
private:
    Application* m_data;
    int m_cbit;
    int m_cbyte;
    ps_color_format m_format;
    int m_screen_width;
    int m_screen_height;
};

void SetImeStatus_platform(bool b);
bool ImeIsShow_platform(void);
void System_init(void);
int get_virtual_key(int pk);

#endif /*_APPLICATION_QT4_H_*/

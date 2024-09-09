/* main.cpp - Agave application 
 * 
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "main_win.h"

static int g_width;
static int g_height;
static const char *home_url;

extern void init_callback(void);
static int get_virtual_key(int pk);

class GtkTimer
{
public:
    GtkTimer() : tid(0) {}
    virtual ~GtkTimer() { g_source_remove(tid); }

    void start(int ms)
    {
        tid = g_timeout_add(ms, GtkTimer::GtkTimerCb, this);
    }

    static gboolean GtkTimerCb(gpointer data)
    {
        GtkTimer* t = (GtkTimer*)data;
        t->timerEvent();
        return TRUE;
    }
protected:
    virtual void timerEvent(void) = 0;
private:
    gint tid;
};

class MsgTimer : public GtkTimer
{
public:
    virtual ~MsgTimer() {}
protected:
    virtual void timerEvent(void)
    {
	    macross_event_dispatch();
    }
};

class UserTimer : public GtkTimer
{
public:
	UserTimer(void(*func)(void*), void* data)
		: m_func(func)
		, m_data(data)
	{
	}
    virtual ~UserTimer() {}
protected:
    virtual void timerEvent(void)
    {
	    m_func(m_data);
    }
private:
	void(*m_func)(void*);
	void* m_data;
};

void MainWindow::destroy(GtkWidget *widget, gpointer data)
{
    MainWindow * mainWindow = (MainWindow*)data;
    delete mainWindow;
    macross_shutdown();
}

gboolean MainWindow::expose(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    MainWindow * mainWindow = (MainWindow*)data;
	GdkRectangle * r = &event->area;
    mainWindow->onDraw(r->x, r->y, r->width, r->height);
    return FALSE;
}

void MainWindow::sizeChange(GtkWidget* widget, GtkAllocation* allocation, gpointer data)
{
    MainWindow * mainWindow = (MainWindow*)data;
	int width = allocation->width;
	int height = allocation->height;
    mainWindow->onSize(width, height);
}

gboolean MainWindow::keyPress(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    MainWindow * mainWindow = (MainWindow*)data;
	unsigned int key = 0;
	if (event->state & GDK_SHIFT_MASK)
		key |= MF_SHIFT;
	if (event->state & GDK_CONTROL_MASK)
		key |= MF_CTRL;

	on_key(mainWindow->getView(), EVT_KEY_DOWN, (MC_VIRTUAL_KEY)get_virtual_key(event->keyval), key);
    return FALSE;
}

gboolean MainWindow::keyRelease(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    MainWindow * mainWindow = (MainWindow*)data;
	unsigned int key = 0;
	if (event->state & GDK_SHIFT_MASK)
		key |= MF_SHIFT;
	if (event->state & GDK_CONTROL_MASK)
		key |= MF_CTRL;

	MC_VIRTUAL_KEY code = (MC_VIRTUAL_KEY)get_virtual_key(event->keyval);
	on_key(mainWindow->getView(), EVT_KEY_UP, code, key);
	if (!key && (code >= 0x30 && code <= 0x5A) && event->length && event->string)
		on_chars(mainWindow->getView(), event->string);
    return FALSE;
}

gboolean MainWindow::mouseButtonPress(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    MainWindow * mainWindow = (MainWindow*)data;
	MC_MOUSE_BUTTON btn = MOUSE_BTN_NONE;
	unsigned int key = 0;
	if (event->button == 1) {
		btn = MOUSE_BTN_LEFT;
	    if (event->state & 1)
		    key |= MF_SHIFT;
	    if (event->state & 4)
		    key |= MF_CTRL;

	    on_mouse(mainWindow->getView(), EVT_MOUSE_DOWN, btn, event->x, event->y, key);
	} else if(event->button == 3) {
	    on_context_menu(mainWindow->getView(), event->x, event->y, key);
    }
    return FALSE;
}

gboolean MainWindow::mouseButtonRelease(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    MainWindow * mainWindow = (MainWindow*)data;
	MC_MOUSE_BUTTON btn = MOUSE_BTN_NONE;
	unsigned int key = 0;
	if (event->button == 1)
		btn = MOUSE_BTN_LEFT;
	if (event->button == 2)
		btn = MOUSE_BTN_MIDDLE;
	if (event->button == 3)
		btn = MOUSE_BTN_RIGHT;
	if (event->state & 1)
		key |= MF_SHIFT;
	if (event->state & 4)
		key |= MF_CTRL;

	on_mouse(mainWindow->getView(), EVT_MOUSE_UP, btn, event->x, event->y, key);
    return FALSE;
}

gboolean MainWindow::mouseMotionNotify(GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
    MainWindow * mainWindow = (MainWindow*)data;
	unsigned int key = 0;
	MC_MOUSE_BUTTON btn = MOUSE_BTN_NONE;
	if (event->state & 1)
		key |= MF_SHIFT;
	if (event->state & 4)
		key |= MF_CTRL;

	if (event->state & 256)
		btn = MOUSE_BTN_LEFT;
	if (event->state & 512)
		btn = MOUSE_BTN_MIDDLE;
	if (event->state & 1024)
		btn = MOUSE_BTN_RIGHT;

	on_mouse(mainWindow->getView(), EVT_MOUSE_MOVE, btn, event->x, event->y, key);
    return FALSE;
}

gboolean MainWindow::focusInEvent(GtkWidget *widget, GdkEventFocus *event, gpointer data)
{
    MainWindow * mainWindow = (MainWindow*)data;
	on_focus(mainWindow->getView());
    return FALSE;
}

gboolean MainWindow::focusOutEvent(GtkWidget *widget, GdkEventFocus *event, gpointer data)
{
    MainWindow * mainWindow = (MainWindow*)data;
	on_lose_focus(mainWindow->getView());
    return FALSE;
}

MainWindow* window = 0;

GtkWidget* getMainWindow()
{
	return window->getWindow();
}

extern "C" MaCrossView* getMainView(void)
{
	return window->getView();
}

MaCrossView* OpenWindow(const char* url, int w, int h)
{
   	MainWindow* win = new MainWindow(w, h);
    win->setWindowTitle("Agave Browser");
	win->loadUrl(url);
	win->show();

	return win->getView();
}

int main(int argc, char* argv[])
{
    if (argv[1])
        home_url = argv[1];

	g_width = 1024;
	g_height = 768;
	macross_initialize(PIXEL_FORMAT_RGB24, g_width, g_height);
	init_callback();

    gtk_init(&argc, &argv);

	MsgTimer * timer = new MsgTimer;
	timer->start(10);

   	window = new MainWindow(g_width, g_height);
    window->setWindowTitle("Agave Browser");
	window->loadUrl(home_url);
	window->show();

    gtk_main();
	return 0;
}

typedef struct {
    int vk;
    int pk;
}KeyEntities;

static KeyEntities key_map[] = {
    {KEY_BACK, GDK_BackSpace},
    {KEY_TAB, GDK_Tab},
    {KEY_CLEAR, GDK_Clear},
    {KEY_ENTER, GDK_Return},
    {KEY_SHIFT, GDK_Shift_L},
    {KEY_CTRL, GDK_Control_L},
    {KEY_ALT, GDK_Menu},
    {KEY_PAUSE, GDK_Pause},
    {KEY_CAPSLOCK, GDK_Caps_Lock},
    {KEY_ESCAPE, GDK_Escape},
    {KEY_SPACE, GDK_space},
    {KEY_PAGEUP, GDK_Prior},
    {KEY_PAGEDOWN, GDK_Next},
    {KEY_END, GDK_End},
    {KEY_HOME, GDK_Home},
    {KEY_LEFT, GDK_Left},
    {KEY_UP, GDK_Up},
    {KEY_RIGHT, GDK_Right},
    {KEY_DOWN, GDK_Down},
    {KEY_SELECT, GDK_Select},
    {KEY_PRINT, GDK_Print},
    {KEY_EXEC, GDK_Execute},
    {KEY_INSERT, GDK_Insert},
    {KEY_DELETE, GDK_Delete},
    {KEY_HELP, GDK_Help},
    {KEY_0, GDK_0},
    {KEY_1, GDK_1},
    {KEY_2, GDK_2},
    {KEY_3, GDK_3},
    {KEY_4, GDK_4},
    {KEY_5, GDK_5},
    {KEY_6, GDK_6},
    {KEY_7, GDK_7},
    {KEY_8, GDK_8},
    {KEY_9, GDK_9},
    {KEY_A, GDK_a},
    {KEY_B, GDK_b},
    {KEY_C, GDK_c},
    {KEY_D, GDK_d},
    {KEY_E, GDK_e},
    {KEY_F, GDK_f},
    {KEY_G, GDK_g},
    {KEY_H, GDK_h},
    {KEY_I, GDK_i},
    {KEY_J, GDK_j},
    {KEY_K, GDK_k},
    {KEY_L, GDK_l},
    {KEY_M, GDK_m},
    {KEY_N, GDK_n},
    {KEY_O, GDK_o},
    {KEY_P, GDK_p},
    {KEY_Q, GDK_q},
    {KEY_R, GDK_r},
    {KEY_S, GDK_s},
    {KEY_T, GDK_t},
    {KEY_U, GDK_u},
    {KEY_V, GDK_v},
    {KEY_W, GDK_w},
    {KEY_X, GDK_x},
    {KEY_Y, GDK_y},
    {KEY_Z, GDK_z},
    {KEY_LWIN, GDK_Meta_L},
    {KEY_RWIN, GDK_Meta_R},
    {KEY_EQUAL, GDK_equal},
    {KEY_MINUS, GDK_minus},
    {KEY_DECIMAL, GDK_decimalpoint},
    {KEY_DIVIDE, GDK_division},
    {KEY_F1, GDK_F1},
    {KEY_F2, GDK_F2},
    {KEY_F3, GDK_F3},
    {KEY_F4, GDK_F4},
    {KEY_F5, GDK_F5},
    {KEY_F6, GDK_F6},
    {KEY_F7, GDK_F7},
    {KEY_F8, GDK_F8},
    {KEY_F9, GDK_F9},
    {KEY_F10, GDK_F10},
    {KEY_F11, GDK_F11},
    {KEY_F12, GDK_F12},
    {KEY_F13, GDK_F13},
    {KEY_F14, GDK_F14},
    {KEY_F15, GDK_F15},
    {KEY_F16, GDK_F16},
    {KEY_F17, GDK_F17},
    {KEY_F18, GDK_F18},
    {KEY_F19, GDK_F19},
    {KEY_F20, GDK_F20},
    {KEY_F21, GDK_F21},
    {KEY_F22, GDK_F22},
    {KEY_F23, GDK_F23},
    {KEY_F24, GDK_F24},
};

static int get_virtual_key(int pk)
{
	int i;
	for(i = 0; i < (sizeof(key_map)/sizeof(KeyEntities)); i++)
		if (key_map[i].pk == pk)
			return key_map[i].vk;
	return KEY_UNKNOWN; 
}

void platform_exception(void(*dump)(int))
{
}

extern "C" void* set_timer(unsigned int mscend, void(*func)(void*), void* data)
{
	UserTimer* timer = new UserTimer(func, data);
	timer->start(mscend);
	return reinterpret_cast<void*>(timer);
}

extern "C" void kill_timer(void* id)
{
	UserTimer* timer = reinterpret_cast<UserTimer*>(id);
	delete timer;
}

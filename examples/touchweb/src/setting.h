/* setting.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _SETTING_H_
#define _SETTING_H_

#include "dashlayer.h"
#include "macross.h"

class MainWindow;

class Setting : public DashLayer
{
public:
	Setting(Widget* parent);
	virtual ~Setting();

	virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
	virtual void OnPaint(ps_context* gc, const Rect* r);
	virtual void OnMouseEvent(const MouseEvent* e);
	virtual void OnShow(void);

	virtual void OnCommit(void);
	void setMainWindow(MainWindow* main);
private:
	void draw_button(ps_context* gc, const Rect& r, bool enable, bool highlight);
	void draw_about_button(ps_context* gc, const Rect& r, bool highlight);
	void draw_user_button(ps_context* gc, const Rect& r, bool highlight);
	void draw_buttons(ps_context* gc, const Rect& r);
	void button_event(void * p);
	MainWindow* m_main;
	Rect m_imgshow;
	Rect m_script;
	Rect m_cookie;
	Rect m_antitxt;
	Rect m_userdata;
	Rect m_about;
	Rect m_event;
	int m_btn;
	MC_CONFIG* m_config;
	bool m_refresh;

};

#endif/*_SETTING_H_*/

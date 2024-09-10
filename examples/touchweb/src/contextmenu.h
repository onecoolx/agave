/* contextmenu.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _CONTEXTMENU_H_
#define _CONTEXTMENU_H_

#include <string>
#include "topmost.h"

class MainWindow;

class ContextMenu : public TopWidget
{
public:
	ContextMenu(Widget* parent);
	virtual ~ContextMenu();

	virtual void OnPaint(ps_context* gc, const Rect* r);
	virtual void OnMouseEvent(const MouseEvent* e);

	void clear(void);
	void create(void);
	void setLinkText(const std::string& link) { m_link = link; }
	void setImgLinkText(const std::string& imglink) { m_imglink = imglink; }

	void setEasyMode(bool b);
	void setMainWindow(MainWindow* main);

	void setValX(int x) { m_x = x; }
	void setValY(int y) { m_y = y; }
	int vx() const { return m_x; }
	int vy() const { return m_y; }
private:
	void button_event(void* p);
	MainWindow* m_main;
	std::string	m_link;
	std::string m_imglink;
	int m_x;
	int m_y;
	int m_btn;
	Rect m_open;
	Rect m_nwin;
	Rect m_bopen;
	Rect m_imgopen;
	Rect m_imgwin;
	Rect m_imgbopen;
	Rect m_event;
	bool m_easy;
};

#endif/*_CONTEXTMENU_H_*/

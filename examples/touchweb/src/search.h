/* search.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _SEARCH_H_
#define _SEARCH_H_

#include <vector>
#include "imclient.h"

class LineEdit;
class MainWindow;

class Search : public ImClient
{
public:
	typedef struct {
		int id;
		ustring name;
		Rect rect;
		unsigned char* icon;
		int w, h, p;
		ps_color c;
		bool page_fold;
	}SeItem;

	Search(Widget* parent);
	virtual ~Search();

	virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
	virtual void OnPaint(ps_context* gc, const Rect* r);
	virtual void OnMouseEvent(const MouseEvent* e);
	virtual void OnShow(void);
	virtual void OnHide(void);

	void setMainWindow(MainWindow* main);
private:
	void draw_sebtn(ps_context* gc, const ustring& text, const Rect& r, bool enable, bool highlight);
	void draw_engine_btns(ps_context* gc);
	void button_search(void * p);
	void button_set(void * p);
	std::vector<SeItem> m_items;
	MainWindow* m_main;
	LineEdit* m_words;
	Rect m_sebtn;
	Rect m_event;
	int m_btn;
	int m_ebtn;
};

#endif/*_SEARCH_H_*/


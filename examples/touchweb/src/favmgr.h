/* favmgr.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _FAVORITES_MANAGER_H_
#define _FAVORITES_MANAGER_H_

#include <vector>
#include <string>
#include "imclient.h"
#include "scrollview.h"

class LineEdit;
class MainWindow;
class FavoritesManager;

class FavoritesPanel : public Widget
{
public:
	FavoritesPanel(Widget* parent, FavoritesManager* mgr);
	virtual ~FavoritesPanel();

	virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
	virtual void OnPaint(ps_context* gc, const Rect* r);
	virtual void OnMouseEvent(const MouseEvent* e);
	virtual void OnShow(void);
	virtual void OnHide(void);

	void setMainWindow(MainWindow* main);
private:
	FavoritesManager* m_favmgr;
	MainWindow* m_main;
	LineEdit* m_title;
	LineEdit* m_addr;
	LineEdit* m_tags;
	Rect m_commit;
	Rect m_cancel;
	Rect m_event;
	int m_btn;
};

class FavoritesViewImpl;
class FavoritesView : public ScrollView
{
public:
	typedef struct {
		int id;
		Rect rect;
		Rect closebtn;
		ustring title;
		ustring url;
		ustring tags;
	}FavoritesItem;

	FavoritesView(Widget* parent);
	virtual ~FavoritesView();

	virtual void OnPaint(ps_context* gc, const Rect* r);
	virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
	virtual void OnDestroy(void);
	virtual void OnMouseEvent(const MouseEvent* e);
	virtual void OnIdle(void);
	virtual void OnShow(void);

	void refresh(void* p);
	void reset(void* p);

	void setFilter(const ustring& key);
	void initFilter(void);
	void setMainWindow(MainWindow* main);
protected:
	virtual void OnPaintContents(ps_context* gc, const Rect* r);
private:
	void draw_pagebtn(ps_context* gc, const ustring& text, const Rect& r, bool enable, bool highlight);
	void draw_closebtn(ps_context* gc, const Rect& r, bool highlight);
	void draw_icon(ps_context* gc, const ps_rect* r);
	void build(int offset);
	void item_click(void* p);
	FavoritesViewImpl* m_impl;
	MainWindow* m_main;
	std::vector<FavoritesItem> m_items;
	ustring m_filter;
	int m_page;
	Rect m_pageup;
	Rect m_pagedown;
	bool m_enableup;
	bool m_enabledown;
	Rect m_event;
	int m_btn;
	int m_pagebtn;
	int m_cbtn;
	int m_startX;
	int m_startY;
	bool m_capture;
	bool m_drag;
	//smooth scroll
	volatile int m_ticket;
	volatile int m_time;
	int m_drx;
	int m_dry;
	int m_scx;
	int m_scy;
};

class FavoritesManager : public ImClient
{
public:
	FavoritesManager(Widget* parent);
	virtual ~FavoritesManager();

	virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
	virtual void OnPaint(ps_context* gc, const Rect* r);
	virtual void OnMouseEvent(const MouseEvent* e);
	virtual void OnShow(void);
	virtual void OnHide(void);

	void hidePanel(void);
	void setMainWindow(MainWindow* main);
private:
	friend class FavoritesPanel;
	friend class FavoritesView;
	MainWindow* m_main;
	LineEdit* m_fltbox;
	FavoritesView* m_view;
	FavoritesPanel* m_panel;
	Rect m_listbtn;
	Rect m_addbtn;
	Rect m_event;
	int m_btn;
};


#endif/*_FAVORITES_MANAGER_H_*/

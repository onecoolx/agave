/* tabpage.cpp - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <picasso/picasso.h>

#include "tabpage.h"
#include "webview.h"
#include "mainwindow.h"
#include "addressbar.h"
#include "toolbar.h"
#include "floatbtn.h"
#include "dialog.h"
#include "files.h"
#include "unicode.h"

TabPage::TabPage(Widget* parent)
	: Widget(parent)
	, m_current(0)
	, m_focused(0)
	, m_main(0)
	, m_fullMode(false)
{
}

TabPage::~TabPage()
{
}

void TabPage::OnChar(uchar_t code)
{
	Widget::OnChar(code);
	
	m_current->OnChar(code);
}

void TabPage::OnSetFocus(void)
{
	Widget::OnSetFocus();

	if (m_focused)
		m_focused->setFocus(false);

	m_current->setFocus(true);
	m_focused = m_current;
}

void TabPage::OnKillFocus(void)
{
	Widget::OnKillFocus();

	if (m_focused){
		m_focused->setFocus(false);
		m_focused = 0;
	}
}

void TabPage::OnIdle(void)
{
	Widget::OnIdle();
	for (std::list<WebView*>::iterator it = m_tabs.begin(); it != m_tabs.end(); ++it)
		(*it)->OnIdle();
}

void TabPage::OnUpdate(const Rect* r)
{
	Widget::OnUpdate(r);
	//NOTE: don't call current's OnUpdate method.
}

void TabPage::OnPaint(ps_context* gc, const Rect* r)
{
	Widget::OnPaint(gc, r);
	m_current->OnPaint(gc, r);
}

void TabPage::OnKeyEvent(const KeyEvent* e)
{
	Widget::OnKeyEvent(e);
	m_current->OnKeyEvent(e);
}

void TabPage::OnMouseEvent(const MouseEvent* e)
{
	Widget::OnMouseEvent(e);
	//set webview focus
	if ((e->btns() & MouseEvent::LeftBtn) && e->type() == MouseEvent::MouseDown)
	{
		m_current->setFocus(true);
	}
	m_current->OnMouseEvent(e);
}

void TabPage::setMainWindow(MainWindow* main)
{
	m_main = main;
}

void TabPage::OnCreate(uint32_t flags, int x, int y, int w, int h)
{
	Widget::OnCreate(flags, x, y, w, h);

    if (m_main->homeUrl())
	    newView(m_main->homeUrl());
    else
		newView(std::string());
}

void TabPage::OnDestroy(void)
{
	Widget::OnDestroy();
	for (std::list<WebView*>::iterator it = m_tabs.begin(); it != m_tabs.end(); it++) {
		(*it)->OnDestroy();
		delete (*it);
	}
	m_tabs.clear();
}

bool TabPage::newView(const std::string& url)
{
	WebView::init();

	if (m_tabs.size() == MAX_TABS) {
		Dialog::AlertBox(m_main, U("ÎÞ·¨´ò¿ª¸ü¶àÒ³Ãæ"), U("¾¯¸æ"));
		return false;
	}

	if (!WebView::haveMemory()) {
		Dialog::AlertBox(m_main, U("ÄÚ´æ²»×ã,ÎÞ·¨´ò¿ª¸ü¶àÒ³Ãæ"), U("¾¯¸æ"));
		return false;
	}

	WebView* view = new WebView(this);
	view->setMainWindow(m_main);
	view->OnCreate(WF_VISIBLE|WF_ENABLED|WF_FOCUSABLE|WF_EDITABLE, 0, 0, width(), height());
	m_current = view;
	m_tabs.push_back(view);

	if (!url.empty()) {
		m_current->loadUrl(url);
	} else {
		ustring dir = File::GetCurrentDir();
		ustring path = ustring(U("file:///"));
		path += dir;
		path += ustring(U("/index.html"));
		std::string url = Unicode::ConvertUTF16ToUTF8(path);
		m_current->loadUrl(url);
		m_current->setLocation(ustring());
	}

	m_main->getToolBar()->notifyButtonUpdate(ToolBar::PageBtn);
	return true;
}

bool TabPage::newViewBack(const std::string& url)
{
	WebView::init();

	if (m_tabs.size() == MAX_TABS) {
		Dialog::AlertBox(m_main, U("ÎÞ·¨´ò¿ª¸ü¶àÒ³Ãæ"), U("¾¯¸æ"));
		return false;
	}

	if (!WebView::haveMemory()) {
		Dialog::AlertBox(m_main, U("ÄÚ´æ²»×ã,ÎÞ·¨´ò¿ª¸ü¶àÒ³Ãæ"), U("¾¯¸æ"));
		return false;
	}

	WebView* view = new WebView(this);
	view->setMainWindow(m_main);
	view->OnCreate(WF_VISIBLE|WF_ENABLED|WF_FOCUSABLE, 0, 0, width(), height());
	m_tabs.push_back(view);

	if (!url.empty())
		view->loadUrl(url);

	m_main->getToolBar()->notifyButtonUpdate(ToolBar::PageBtn);
	return true;
}

void TabPage::closeView(WebView* view)
{
	if (!view)
		return;

	m_tabs.remove(view);
	
	if (m_current == view) {
		if (!m_tabs.empty()) {
			m_current = *(m_tabs.begin());
			locationChanged(m_current);
		} else {
			m_current = 0;
		}
		Update(NULL);
	}

	view->OnDestroy();
	delete view;

	if (!m_tabs.size()) {
		newView(std::string());
	} else {
		m_main->getToolBar()->notifyButtonUpdate(ToolBar::PageBtn);
	}
}

WebView* TabPage::getActiveView(void) const 
{
	return m_current;
}

void TabPage::setActiveView(WebView* view)
{
	m_current = view;
	locationChanged(m_current);
	Update(NULL);
}

void TabPage::locationChanged(WebView* view)
{
	if (view && (view == m_current)) {
		m_main->getAddressBar()->setText(view->location());
		m_main->getAddressBar()->setUpdate();
		m_main->getToolBar()->notifyButtonUpdate(ToolBar::ZoomBtn);
	}
}

bool TabPage::isActiveView(const WebView* view) const
{
   	return m_current == view;
}

size_t TabPage::getCount(void) const
{
	return m_tabs.size();
}

std::list<WebView*> TabPage::getTabs(void) const
{
	return m_tabs;
}

void TabPage::sizeChanged(int x, int y, int w, int h)
{
	setX(x);
	setY(y);
	setWidth(w);
	setHeight(h);
	m_current->setSize(w, h);
}

void TabPage::setFullMode(bool full)
{
	if (m_fullMode != full) {
		if (full){
			m_main->getAddressBar()->Hide();
			m_main->getToolBar()->Hide();

			m_main->getModeBtn()->Show();
			m_main->getBackBtn()->Show();

			m_current->setFullMode(true);
			sizeChanged(0, 0, width(), height()+TOOLBAR_HEIGHT*2);
		} else {
			m_main->getAddressBar()->Show();
			m_main->getToolBar()->Show();

			m_main->getModeBtn()->Hide();
			m_main->getBackBtn()->Hide();

			m_current->setFullMode(false);
			sizeChanged(0, TOOLBAR_HEIGHT, width(), height()-TOOLBAR_HEIGHT*2);
		}
	}
	m_fullMode = full;
}

bool TabPage::isFullMode(void) const 
{
	return m_fullMode;
}

void TabPage::repaintAllPage(void* p)
{
	for (std::list<WebView*>::iterator it = m_tabs.begin(); it != m_tabs.end(); ++it)
		(*it)->reFresh();
}


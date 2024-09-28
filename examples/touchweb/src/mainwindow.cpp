/* mainwindow.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */
#include "config.h"

#include <string.h>
#include <picasso/picasso.h>

#include "widget.h"
#include "toolbar.h"
#include "tabpage.h"
#include "dialog.h"
#include "addressbar.h"
#include "contextmenu.h"
#include "mainwindow.h"
#include "application.h"
#include "ctrlpanel.h"
#include "netstat.h"
#include "network.h"
#include "pagemgr.h"
#include "setting.h"
#include "about.h"
#include "search.h"
#include "hismgr.h"
#include "favmgr.h"
#include "secmgr.h"
#include "fillmgr.h"
#include "extracfg.h"
#include "floatbtn.h"

#if defined(WIN32)
    #include "mainwindow_win32.h"
#endif

#ifdef QT5
    #include "mainwindow_qt5.h"
#endif

ps_font* MainWindow::m_uifont = 0;
ps_font* MainWindow::m_uibfont = 0;
bool MainWindow::m_font_antialias = false;

MainWindow::MainWindow(Widget* parent)
    : Widget(parent)
    , m_impl(0)
    , m_toolbar(0)
    , m_itoolbar(0)
    , m_address(0)
    , m_tabs(0)
    , m_panel(0)
    , m_netstat(0)
    , m_pagemgr(0)
    , m_setting(0)
    , m_search(0)
    , m_hismgr(0)
    , m_favmgr(0)
    , m_fillmgr(0)
    , m_secmgr(0)
    , m_about(0)
    , m_menu(0)
    , m_modebtn(0)
    , m_backbtn(0)
    , m_home_url(0)
{
    m_impl = new MainWindowImpl(this);
    //window widgets
    m_toolbar = new ToolBar(this);
    m_itoolbar = new ImToolBar(this);
    m_address = new AddressBar(this);
    m_tabs = new TabPage(this);
    m_panel = new CtrlPanel(this);
    m_netstat = new NetStatPanel(this);
    m_pagemgr = new PageManager(this);
    m_setting = new Setting(this);
    m_search = new Search(this);
    m_hismgr = new HistoryManager(this);
    m_favmgr = new FavoritesManager(this);
    m_fillmgr = new AutoFillManager(this);
    m_secmgr = new SecirtyData(this);
    m_about = new About(this);
    m_menu = new ContextMenu(this);

    m_modebtn = new ModeButton(this);
    m_backbtn = new BackButton(this);
}

MainWindow::~MainWindow()
{
    if (m_home_url) {
        delete [] m_home_url;
    }

    delete m_impl;
    delete m_backbtn;
    delete m_modebtn;
    delete m_menu;
    delete m_about;
    delete m_secmgr;
    delete m_fillmgr;
    delete m_favmgr;
    delete m_hismgr;
    delete m_search;
    delete m_setting;
    delete m_pagemgr;
    delete m_netstat;
    delete m_panel;
    delete m_tabs;
    delete m_address;
    delete m_itoolbar;
    delete m_toolbar;
}

bool MainWindow::Create(void* hInst)
{
    return m_impl->Create(hInst, "Agave", 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

bool MainWindow::Destroy(void)
{
    if (Dialog::ConfirmBox(this, U("Are you sure you want to exit?"), U("Information"))) {
        m_impl->Destroy();
        return true;
    }
    return false;
}

//Event Handler
void MainWindow::OnPaint(ps_context* gc, const Rect* r)
{
    Widget::OnPaint(gc, r);
    //TODO: add paint code here.
}

void MainWindow::OnUpdate(const Rect* r)
{
    Widget::OnUpdate(r);
    m_impl->OnUpdate(r->x, r->y, r->w, r->h);
}

void MainWindow::OnCreate(uint32_t f, int x, int y, int w, int h)
{
    Widget::OnCreate(WF_VISIBLE | WF_ENABLED | WF_FOCUSABLE, x, y, w, h);
    //TODO: add create code here.
    m_address->setMainWindow(this);
    m_tabs->setMainWindow(this);
    m_toolbar->setMainWindow(this);
    m_itoolbar->setMainWindow(this);
    m_panel->setMainWindow(this);
    m_netstat->setMainWindow(this);
    m_pagemgr->setMainWindow(this);
    m_setting->setMainWindow(this);
    m_search->setMainWindow(this);
    m_hismgr->setMainWindow(this);
    m_favmgr->setMainWindow(this);
    m_fillmgr->setMainWindow(this);
    m_secmgr->setMainWindow(this);
    m_about->setMainWindow(this);
    m_menu->setMainWindow(this);
    m_modebtn->setMainWindow(this);
    m_backbtn->setMainWindow(this);

    m_address->OnCreate(WF_VISIBLE | WF_ENABLED | WF_FOCUSABLE | WF_EDITABLE, 0, 0, w, TOOLBAR_HEIGHT);
    m_tabs->OnCreate(WF_VISIBLE | WF_ENABLED | WF_FOCUSABLE | WF_EDITABLE, 0, TOOLBAR_HEIGHT, w, h - TOOLBAR_HEIGHT * 2);
    m_toolbar->OnCreate(WF_VISIBLE | WF_ENABLED, 0, h - TOOLBAR_HEIGHT, w, TOOLBAR_HEIGHT);
    m_itoolbar->OnCreate(WF_ENABLED, 0, h - TOOLBAR_HEIGHT, w, TOOLBAR_HEIGHT);
    m_panel->OnCreate(WF_ENABLED, 0, h - 150 * TOOLBAR_HEIGHT / 30, w, 120 * TOOLBAR_HEIGHT / 30);
    m_netstat->OnCreate(WF_ENABLED, w / 10, w / 25 * 12, w / 5 * 4, w / 25 * 12);
    m_pagemgr->OnCreate(WF_ENABLED, 0, h / 2, w, h / 2 - TOOLBAR_HEIGHT);
    m_setting->OnCreate(WF_ENABLED, 0, 0, w, h);
    m_search->OnCreate(WF_ENABLED, 0, 0, w, h);
    m_hismgr->OnCreate(WF_ENABLED, 0, 0, w, h);
    m_favmgr->OnCreate(WF_ENABLED, 0, 0, w, h);
    m_secmgr->OnCreate(WF_ENABLED, 0, 0, w, h);
    m_fillmgr->OnCreate(WF_ENABLED, 0, h - 120 * TOOLBAR_HEIGHT / 45 - TOOLBAR_HEIGHT, w, 120 * TOOLBAR_HEIGHT / 45);
    m_about->OnCreate(WF_ENABLED, 0, 0, w, h);
    m_menu->OnCreate(WF_ENABLED, 0, 0, 0, 0);

    m_modebtn->OnCreate(WF_ENABLED, TOOLBAR_HEIGHT / 3, h - TOOLBAR_HEIGHT / 6 * 5 - TOOLBAR_HEIGHT / 3,
                        TOOLBAR_HEIGHT / 6 * 5, TOOLBAR_HEIGHT / 6 * 5);

    m_backbtn->OnCreate(WF_ENABLED, w - TOOLBAR_HEIGHT / 6 * 5 - TOOLBAR_HEIGHT / 3,
                        h - TOOLBAR_HEIGHT / 6 * 5 - TOOLBAR_HEIGHT / 3,
                        TOOLBAR_HEIGHT / 6 * 5, TOOLBAR_HEIGHT / 6 * 5);

    addChild(m_address);
    addChild(m_tabs);
    addChild(m_toolbar);
    addChild(m_itoolbar);
    addChild(m_panel);
    addChild(m_netstat);
    addChild(m_pagemgr);
    addChild(m_setting);
    addChild(m_search);
    addChild(m_hismgr);
    addChild(m_favmgr);
    addChild(m_fillmgr);
    addChild(m_secmgr);
    addChild(m_about);
    addChild(m_menu);
    addChild(m_modebtn);
    addChild(m_backbtn);
}

void MainWindow::OnDestroy(void)
{
    Widget::OnDestroy();
    //TODO: add destroy code here.
}

void MainWindow::setHomeUrl(const char* url)
{
    if (m_home_url) {
        delete [] m_home_url;
        m_home_url = 0;
    }

    if (url) {
        size_t len = strlen(url);
        if (len) {
            m_home_url = new char[len + 1];
            strcpy(m_home_url, url);
        }
    }
}

void MainWindow::netCheck(void)
{
    if (!Application::getInstance()->getNetService()->isConnected()) {
        Application::getInstance()->getMainWindow()->getNetPanel()->startNetWorkCheck();
    } else {
        if (Application::getInstance()->getExtraConfig()->needReport()) {
            Application::getInstance()->getExtraConfig()->setNeedReport(false);
            // add report code here
            Application::getInstance()->report();
        }
    }
}

void MainWindow::Paint(ps_context* gc, const Rect* rc)
{
    Widget* layer = NULL;
    if (m_setting->isVisible()) {
        layer = m_setting;
    } else if (m_search->isVisible()) {
        layer = m_search;
    } else if (m_about->isVisible()) {
        layer = m_about;
    } else if (m_secmgr->isVisible()) {
        layer = m_secmgr;
    } else if (m_hismgr->isVisible()) {
        layer = m_hismgr;
    } else if (m_favmgr->isVisible()) {
        layer = m_favmgr;
    } //hack for dashlayer draw

    if (layer) {
        layer->Paint(gc, rc);
    } else {
        Widget::Paint(gc, rc);
    }
}

//ststic method
ps_font* MainWindow::getUIFont(void)
{
    if (!m_uifont) {
        m_uifont = ps_font_create("sans-serif", CHARSET_UNICODE, DASH_TITLE_HEIGHT / 5 * 2, FONT_WEIGHT_REGULAR, False);
    }
    return m_uifont;
}

ps_font* MainWindow::getUIBoldFont(void)
{
    if (!m_uibfont) {
        m_uibfont = ps_font_create("sans-serif", CHARSET_UNICODE, DASH_TITLE_HEIGHT / 5 * 2, FONT_WEIGHT_BOLD, False);
    }
    return m_uibfont;
}

void MainWindow::setUIFontAntialias(bool b)
{
    m_font_antialias = b;
}

bool MainWindow::uiFontAntialias(void)
{
    return m_font_antialias;
}

bool MainWindow::getChooseFile(uchar_t* name, unsigned int len)
{
    return m_impl->getChooseFile(name, len);
}

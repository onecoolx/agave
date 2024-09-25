/* mainwindow.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "widget.h"

class ToolBar;
class ImToolBar;
class TabPage;
class AddressBar;
class CtrlPanel;
class NetStatPanel;
class ContextMenu;
class PageManager;
class Setting;
class About;
class Search;
class HistoryManager;
class FavoritesManager;
class AutoFillManager;
class SecirtyData;
class MainWindowImpl;
class ModeButton;
class BackButton;

class MainWindow : public Widget
{
public:
    MainWindow(Widget* parent = 0);
    virtual ~MainWindow();

    bool Create(void* hInst);
    bool Destroy(void);
    void Paint(ps_context* gc, const Rect* rc);
    //event handle
    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnCreate(uint32_t f, int x, int y, int w, int h);
    virtual void OnDestroy(void);
    virtual void OnUpdate(const Rect* r);

    void setHomeUrl(const char* url);
    char* homeUrl(void) const { return m_home_url; }

    void netCheck(void);

    MainWindowImpl* platform(void) const { return m_impl; }
    TabPage* getTabs(void) const { return m_tabs; }
    CtrlPanel* getCtrlPanel(void) const { return m_panel; }
    NetStatPanel* getNetPanel(void) const { return m_netstat; }
    PageManager* getPageManager(void) const { return m_pagemgr; }
    AddressBar* getAddressBar(void) const { return m_address; }
    ContextMenu* getContextMenu(void) const { return m_menu; }
    ToolBar* getToolBar(void) const { return m_toolbar; }
    ImToolBar* getImToolBar(void) const { return m_itoolbar; }

    ModeButton* getModeBtn(void) const { return m_modebtn; }
    BackButton* getBackBtn(void) const { return m_backbtn; }

    Setting* getSetting(void) const { return m_setting; }
    HistoryManager* getHistory(void) const { return m_hismgr; }
    FavoritesManager* getFavorites(void) const { return m_favmgr; }
    AutoFillManager* getAutoFill(void) const { return m_fillmgr; }
    Search* getSearch(void) const { return m_search; }
    About* getAbout(void) const { return m_about; }
    SecirtyData* getSecirtyData(void) const { return m_secmgr; }

    static ps_font* getUIFont(void);
    static ps_font* getUIBoldFont(void);
    static void setUIFontAntialias(bool b);
    static bool uiFontAntialias(void);
public:
    bool getChooseFile(uchar_t* name, unsigned int len);
private:
    MainWindowImpl* m_impl;
    ToolBar* m_toolbar;
    ImToolBar* m_itoolbar;
    AddressBar* m_address;
    TabPage* m_tabs;
    CtrlPanel* m_panel;
    NetStatPanel* m_netstat;
    PageManager* m_pagemgr;
    Setting* m_setting;
    Search* m_search;
    HistoryManager* m_hismgr;
    FavoritesManager* m_favmgr;
    AutoFillManager* m_fillmgr;
    SecirtyData* m_secmgr;
    About* m_about;
    ContextMenu* m_menu;
    ModeButton* m_modebtn;
    BackButton* m_backbtn;
    char* m_home_url;

    static ps_font* m_uifont;
    static ps_font* m_uibfont;
    static bool m_font_antialias;
};

#endif/*_MAINWINDOW_H_*/

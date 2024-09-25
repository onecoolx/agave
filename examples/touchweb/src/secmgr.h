/* secmgr.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _SECURITY_MANAGER_H_
#define _SECURITY_MANAGER_H_

#include "dashlayer.h"

class MainWindow;
class ColorButton;

class SecirtyData : public DashLayer
{
public:
    SecirtyData(Widget* parent);
    virtual ~SecirtyData();

    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnShow(void);

    void setMainWindow(MainWindow* main);

private:
    void clear_cookies(void);
    void clear_cache(void);
    void clear_passwd(void);
    MainWindow* m_main;
    ColorButton* m_clearCookie;
    ColorButton* m_clearCache;
    ColorButton* m_clearPasswd;
};

#endif /*_SECURITY_MANAGER_H_*/

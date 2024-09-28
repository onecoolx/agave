/* dialog.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _DIALOG_H_
#define _DIALOG_H_

#include <string>
#include "widget.h"

class DialogImpl;
class MainWindow;

class Dialog : public Widget
{
public:
    static void AlertBox(MainWindow* main, const ustring& msg, const ustring& title);
    static bool ConfirmBox(MainWindow* main, const ustring& msg, const ustring& title);
    static ustring PromptBox(MainWindow* main, const ustring& msg,
                             const ustring& defmsg, const ustring& title);
    static bool LoginBox(MainWindow* main, ustring& user, ustring& pwd, const ustring& title);
public:
    Dialog(Widget* parent, MainWindow* main);
    virtual ~Dialog();

    void setTitle(const ustring& title);
    int titleHeight(void) const;

    void Create(int x, int y, int w, int h);
    int Modal(void);
    void EndModal(int exit);

    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
    virtual void OnUpdate(const Rect* r);
protected:
    void Show(void);
    void Hide(void);
private:
    DialogImpl* m_impl;
    MainWindow* m_main;
    ustring m_title;
};

#endif/*_DIALOG_H_*/

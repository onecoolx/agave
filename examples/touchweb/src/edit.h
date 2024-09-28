/* edit.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _EDITWIDGET_H_
#define _EDITWIDGET_H_

#include "widget.h"

class LineEdit : public Widget
{
public:
    LineEdit(Widget* parent);
    virtual ~LineEdit();

    ustring getText(void) const;
    void setText(const ustring& s);

    void setTipText(const ustring& s);
    ustring getTipText(void) const;

    void setPassWordMode(bool b) { m_ispwd = b; }
    bool isPassWordMode(void) const { return m_ispwd; }

    void setClearButton(bool b) { m_hasbtn = b; }
    bool hasClearButton(void) const { return m_hasbtn; }

    void setHasBorder(bool b) { m_border = b; }
    bool hasBorder(void) const { return m_border; }

    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
    virtual void OnSetFocus(void);
    virtual void OnKillFocus(void);
    virtual void OnChar(uchar_t code);
    virtual void OnMouseEvent(const MouseEvent* e);
private:
    ustring m_text;
    ustring m_tip;
    bool m_ispwd;
    bool m_hasbtn;
    bool m_border;
    Rect m_clear;
    int m_btn;
};

#endif/*_EDITWIDGET_H_*/

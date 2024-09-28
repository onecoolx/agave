/* dialog.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */
#include "config.h"
#include <picasso/picasso.h>

#include "dialog.h"
#include "mainwindow.h"
#include "edit.h"

#if defined(WIN32)
    #include "dialog_win32.h"
#endif

#ifdef GTK2
    #include "dialog_gtk2.h"
#endif

#ifdef QT5
    #include "dialog_qt5.h"
#endif

Dialog::Dialog(Widget* parent, MainWindow* main)
    : Widget(parent)
    , m_impl(0)
    , m_main(main)
{
    m_impl = new DialogImpl(this);
}

Dialog::~Dialog()
{
    delete m_impl;
}

void Dialog::Show(void)
{
    Widget::Show();
}

void Dialog::Hide(void)
{
    Widget::Hide();
}

int Dialog::titleHeight(void) const
{
    return TITLE_HEIGHT;
}

void Dialog::setTitle(const ustring& title)
{
    m_title = title;
}

void Dialog::OnUpdate(const Rect* r)
{
    Widget::OnUpdate(r);
    m_impl->OnUpdate(r->x, r->y, r->w, r->h);
}

void Dialog::OnCreate(uint32_t f, int x, int y, int w, int h)
{
    Widget::OnCreate(WF_VISIBLE | WF_ENABLED | WF_TOPMOST | WF_MODAL, x, y, w, h);
}

void Dialog::OnPaint(ps_context* gc, const Rect* d)
{
    Widget::OnPaint(gc, d);
    //draw paint code
    ps_save(gc);

    ps_color c = {0.1, 0.1, 0.1, 1};
    ps_color s = {0.8, 0.8, 0.8, 1};
    ps_color t = {0.2, 0.2, 0.2, 1};
    ps_rect r = {0, 0, width() - 1, height() - 1};
    ps_rectangle(gc, &r);
    ps_set_source_color(gc, &c);
    ps_set_stroke_color(gc, &t);
    ps_paint(gc);

    ps_set_stroke_color(gc, &s);
    ps_point p1 = {0, TITLE_HEIGHT};
    ps_point p2 = {width(), TITLE_HEIGHT};
    ps_move_to(gc, &p1);
    ps_line_to(gc, &p2);
    ps_stroke(gc);

    if (!m_title.empty()) {
        int b = TITLE_HEIGHT / 22;
        ps_set_text_color(gc, &s);
        ps_font* f = MainWindow::getUIFont();
        ps_font* of = ps_set_font(gc, f);
#ifdef FONT_NO_ANTIALIAS
        ps_set_text_antialias(gc, False);
#else
        if (MainWindow::uiFontAntialias()) {
            ps_set_text_antialias(gc, True);
        } else {
            ps_set_text_antialias(gc, False);
        }
#endif
        ps_wide_text_out_length(gc, 10 * b, 4 * b, (ps_uchar16*)m_title.c_str(), m_title.size());
        ps_set_font(gc, of);
    }

    ps_restore(gc);
}

int Dialog::Modal(void)
{
    return m_impl->DoModal();
}

void Dialog::EndModal(int exit)
{
    m_impl->EndModal(exit);
}

void Dialog::Create(int x, int y, int w, int h)
{
    m_impl->Create(m_main->platform(), x, y, w, h);
}

//message dialog
class MsgDialog : public Dialog
{
public:
    MsgDialog(Widget* p, MainWindow* m);
    virtual ~MsgDialog();

    void setMessage(const ustring& msg);
    void setPrompt(bool b);

    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
    virtual void OnMouseEvent(const MouseEvent* e);
private:
    void draw_btns(ps_context* gc);
    void draw_light(ps_context* gc, const Rect& r);
    void draw_btnText(ps_context* gc, const ustring& text, const Rect& r);
    void draw_msgText(ps_context* gc, const Rect& r);
    ustring m_msg;
    bool m_prompt;
    Rect m_commit;
    Rect m_cancel;
    Rect m_textArea;
    int m_btn;
};

MsgDialog::MsgDialog(Widget* p, MainWindow* m)
    : Dialog(p, m)
    , m_prompt(false)
    , m_commit(0, 0, 0, 0)
    , m_cancel(0, 0, 0, 0)
    , m_textArea(0, 0, 0, 0)
    , m_btn(0)
{
}

MsgDialog::~MsgDialog()
{
}

void MsgDialog::setPrompt(bool b)
{
    m_prompt = b;
}

void MsgDialog::setMessage(const ustring& msg)
{
    m_msg = msg;
}

void MsgDialog::draw_msgText(ps_context* gc, const Rect& r)
{
    ps_size sz = {0};
    ps_get_text_extent(gc, m_msg.c_str(), m_msg.length(), &sz);
    ps_size sz1 = {0};
    ps_get_text_extent(gc, m_msg.c_str(), 1, &sz1);
    int max_lines = r.h / sz.h;
    int num_lines = sz.w / r.w + 1;
    int ns = r.w / sz1.w;

    int real_line = MIN(num_lines, max_lines);

    for (int i = 0; i < real_line; i++) {
        int len = MIN(ns, (int)m_msg.length() - i * ns);
        ps_wide_text_out_length(gc, r.x, r.y + i * sz1.h, (ps_uchar16*)m_msg.c_str() + i * ns, len);
    }
}

void MsgDialog::draw_btnText(ps_context* gc, const ustring& text, const Rect& r)
{
    if (!text.empty()) {
        ps_size sz = {0};
        ps_get_text_extent(gc, text.c_str(), text.length(), &sz);
        ps_wide_text_out_length(gc, r.x + (r.w - sz.w) / 2, r.y + r.h / 2 - 6 * (TITLE_HEIGHT / 22),
                                (ps_uchar16*)text.c_str(), text.length());
    }
}

void MsgDialog::draw_light(ps_context* gc, const Rect& r)
{
    int b = TITLE_HEIGHT / 22;
    ps_color tc = {1, 1, 1, 0.8};
    ps_color tc2 = {1, 1, 1, 0.2};
    ps_rect lrc = {r.x + 4 * b, r.y + 4 * b, r.w - 7 * b, r.h / 2 - 4 * b};
    ps_rounded_rect(gc, &lrc, 3 * b, 3 * b, 3 * b, 3 * b, 0, 0, 0, 0);
    ps_point s1 = {lrc.x, lrc.y};
    ps_point e1 = {lrc.x, lrc.y + lrc.h};
    ps_gradient* gt = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s1, &e1);
    ps_gradient_add_color_stop(gt, 0, &tc);
    ps_gradient_add_color_stop(gt, 1, &tc2);
    ps_set_source_gradient(gc, gt);
    ps_fill(gc);
    ps_gradient_unref(gt);
}

void MsgDialog::draw_btns(ps_context* gc)
{
    int b = TITLE_HEIGHT / 22;
    if (!m_commit.isEmpty()) {
        ps_rect rc = {m_commit.x + 2 * b, m_commit.y + 2 * b, m_commit.w - 4 * b, m_commit.h - 4 * b};
        ps_color c = {0.5, 0.5, 0.5, 1};
        ps_set_stroke_color(gc, &c);

        ps_set_line_width(gc, b);
        ps_rounded_rect(gc, &rc, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b);
        ps_stroke(gc);
        ps_set_line_width(gc, 1);

        if (m_btn == 2) {
            ps_rect r2 = {rc.x + b, rc.y + b, rc.w - b, rc.h - b};
            ps_rounded_rect(gc, &r2, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b);

            ps_color c1 = {0, 0, 0, 1};
            ps_color c2 = {0.02, 1, 0, 1};
            ps_point s = {r2.x, r2.y};
            ps_point e = {r2.x, r2.y + r2.h};
            ps_gradient* g = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s, &e);

            ps_gradient_add_color_stop(g, 0, &c1);
            ps_gradient_add_color_stop(g, 1, &c2);

            ps_set_source_gradient(gc, g);
            ps_fill(gc);
            ps_gradient_unref(g);
        }

        draw_btnText(gc, U("Ok"), m_commit);
        draw_light(gc, m_commit);
    }

    if (!m_cancel.isEmpty()) {
        ps_rect rc = {m_cancel.x + 2 * b, m_cancel.y + 2 * b, m_cancel.w - 4 * b, m_cancel.h - 4 * b};
        ps_color c = {0.5, 0.5, 0.5, 1};
        ps_set_stroke_color(gc, &c);

        ps_set_line_width(gc, b);
        ps_rounded_rect(gc, &rc, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b);
        ps_stroke(gc);
        ps_set_line_width(gc, 1);

        if (m_btn == 1) {
            ps_rect r2 = {rc.x + b, rc.y + b, rc.w - b, rc.h - b};
            ps_rounded_rect(gc, &r2, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b);

            ps_color c1 = {0, 0, 0, 1};
            ps_color c2 = {1, 0, 0, 1};
            ps_point s = {r2.x, r2.y};
            ps_point e = {r2.x, r2.y + r2.h};
            ps_gradient* g = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s, &e);

            ps_gradient_add_color_stop(g, 0, &c1);
            ps_gradient_add_color_stop(g, 1, &c2);

            ps_set_source_gradient(gc, g);
            ps_fill(gc);
            ps_gradient_unref(g);
        }

        draw_btnText(gc, U("Cancel"), m_cancel);
        draw_light(gc, m_cancel);
    }
}

void MsgDialog::OnPaint(ps_context* gc, const Rect* d)
{
    Dialog::OnPaint(gc, d);

    ps_save(gc);

    ps_color c = {1, 1, 1, 1};
    ps_set_text_color(gc, &c);
    ps_font* f = MainWindow::getUIFont();
    ps_font* of = ps_set_font(gc, f);
#ifdef FONT_NO_ANTIALIAS
    ps_set_text_antialias(gc, False);
#else
    if (MainWindow::uiFontAntialias()) {
        ps_set_text_antialias(gc, True);
    } else {
        ps_set_text_antialias(gc, False);
    }
#endif

    draw_msgText(gc, m_textArea);

    draw_btns(gc);

    ps_set_font(gc, of);

    ps_restore(gc);
}

void MsgDialog::OnCreate(uint32_t f, int x, int y, int w, int h)
{
    Dialog::OnCreate(f, x, y, w, h);
    int b = TITLE_HEIGHT / 22;
    m_textArea = Rect(5 * b, titleHeight() + 5 * b, w - 10 * b, h - titleHeight() - BUTTON_HEIGHT - 5 * b);

    int btn_width = BUTTON_HEIGHT * 2;
    if (m_prompt) {
        int border = (width() - BUTTON_HEIGHT * 4) / 4;
        m_commit = Rect(border, h - BUTTON_HEIGHT, btn_width, BUTTON_HEIGHT);
        m_cancel = Rect(width() - border - btn_width, h - BUTTON_HEIGHT, btn_width, BUTTON_HEIGHT);
    } else {
        m_commit = Rect((w - btn_width) / 2, h - BUTTON_HEIGHT, btn_width, BUTTON_HEIGHT);
    }
}

void MsgDialog::OnMouseEvent(const MouseEvent* e)
{
    Dialog::OnMouseEvent(e);
    if (e->type() == MouseEvent::MouseDown) {
        if (m_commit.contains(e->x(), e->y())) {
            m_btn = 2;
        } else if (m_cancel.contains(e->x(), e->y())) {
            m_btn = 1;
        } else {
            m_btn = 0;
        }

        if (m_btn) {
            Rect r(0, height() - BUTTON_HEIGHT, width(), BUTTON_HEIGHT);
            Update(&r);
        }
    } else if (e->type() == MouseEvent::MouseUp) {
        if (m_btn) {
            int ret = m_btn;
            m_btn = 0;
            Rect r(0, height() - BUTTON_HEIGHT, width(), BUTTON_HEIGHT);
            Update(&r);
            EndModal(ret);
        }
    }
}

// common dialogs
void Dialog::AlertBox(MainWindow* main, const ustring& msg, const ustring& title)
{
    int width = main->width() / 5 * 4;
    int height = width / 5 * 3;

    MsgDialog dlg(0, main);
    dlg.setTitle(title);
    dlg.setMessage(msg);
    dlg.Create((main->width() - width) / 2, (main->height() - height) / 2, width, height);
    dlg.Modal();
}

bool Dialog::ConfirmBox(MainWindow* main, const ustring& msg, const ustring& title)
{
    int width = main->width() / 5 * 4;
    int height = width / 5 * 3;

    MsgDialog dlg(0, main);
    dlg.setTitle(title);
    dlg.setMessage(msg);
    dlg.setPrompt(true);
    dlg.Create((main->width() - width) / 2, (main->height() - height) / 2, width, height);
    int ret = dlg.Modal();
    if (ret == 1) {
        return false;
    } else {
        return true;
    }
}

//input dialog
class InputDialog : public Dialog
{
public:
    InputDialog(Widget* p, MainWindow* m);
    virtual ~InputDialog();

    void setMessage(const ustring& msg);
    void setUserText(const ustring& msg) { m_user->setText(msg); }
    ustring getUserText(void) const { return m_user->getText(); }

    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
    virtual void OnMouseEvent(const MouseEvent* e);
private:
    void draw_btns(ps_context* gc);
    void draw_light(ps_context* gc, const Rect& r);
    void draw_btnText(ps_context* gc, const ustring& text, const Rect& r);
    void draw_msgText(ps_context* gc, const Rect& r);
    ustring m_msg;
    LineEdit* m_user;
    Rect m_commit;
    Rect m_cancel;
    Rect m_textArea;
    int m_btn;
};

InputDialog::InputDialog(Widget* p, MainWindow* m)
    : Dialog(p, m)
    , m_user(0)
    , m_commit(0, 0, 0, 0)
    , m_cancel(0, 0, 0, 0)
    , m_textArea(0, 0, 0, 0)
    , m_btn(0)
{
    m_user = new LineEdit(this);
}

InputDialog::~InputDialog()
{
    delete m_user;
}

void InputDialog::setMessage(const ustring& msg)
{
    m_msg = msg;
}

void InputDialog::draw_msgText(ps_context* gc, const Rect& r)
{
    ps_size sz = {0};
    ps_get_text_extent(gc, m_msg.c_str(), m_msg.length(), &sz);
    ps_size sz1 = {0};
    ps_get_text_extent(gc, m_msg.c_str(), 1, &sz1);
    int max_lines = r.h / sz.h;
    int num_lines = sz.w / r.w + 1;
    int ns = r.w / sz1.w;

    int real_line = MIN(num_lines, max_lines);

    for (int i = 0; i < real_line; i++) {
        int len = MIN(ns, (int)m_msg.length() - i * ns);
        ps_wide_text_out_length(gc, r.x, r.y + i * sz1.h, (ps_uchar16*)m_msg.c_str() + i * ns, len);
    }
}
void InputDialog::draw_btnText(ps_context* gc, const ustring& text, const Rect& r)
{
    if (!text.empty()) {
        ps_size sz = {0};
        ps_get_text_extent(gc, text.c_str(), text.length(), &sz);
        ps_wide_text_out_length(gc, r.x + (r.w - sz.w) / 2, r.y + r.h / 2 - 6 * (TITLE_HEIGHT / 22),
                                (ps_uchar16*)text.c_str(), text.length());
    }
}

void InputDialog::draw_light(ps_context* gc, const Rect& r)
{
    int b = TITLE_HEIGHT / 22;
    ps_color tc = {1, 1, 1, 0.8};
    ps_color tc2 = {1, 1, 1, 0.2};
    ps_rect lrc = {r.x + 4 * b, r.y + 4 * b, r.w - 7 * b, r.h / 2 - 4 * b};
    ps_rounded_rect(gc, &lrc, 3 * b, 3 * b, 3 * b, 3 * b, 0, 0, 0, 0);
    ps_point s1 = {lrc.x, lrc.y};
    ps_point e1 = {lrc.x, lrc.y + lrc.h};
    ps_gradient* gt = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s1, &e1);
    ps_gradient_add_color_stop(gt, 0, &tc);
    ps_gradient_add_color_stop(gt, 1, &tc2);
    ps_set_source_gradient(gc, gt);
    ps_fill(gc);
    ps_gradient_unref(gt);
}

void InputDialog::draw_btns(ps_context* gc)
{
    int b = TITLE_HEIGHT / 22;
    if (!m_commit.isEmpty()) {
        ps_rect rc = {m_commit.x + 2 * b, m_commit.y + 2 * b, m_commit.w - 4 * b, m_commit.h - 4 * b};
        ps_color c = {0.5, 0.5, 0.5, 1};
        ps_set_stroke_color(gc, &c);

        ps_set_line_width(gc, b);
        ps_rounded_rect(gc, &rc, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b);
        ps_stroke(gc);
        ps_set_line_width(gc, 1);

        if (m_btn == 2) {
            ps_rect r2 = {rc.x + b, rc.y + b, rc.w - b, rc.h - b};
            ps_rounded_rect(gc, &r2, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b);

            ps_color c1 = {0, 0, 0, 1};
            ps_color c2 = {0.02, 1, 0, 1};
            ps_point s = {r2.x, r2.y};
            ps_point e = {r2.x, r2.y + r2.h};
            ps_gradient* g = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s, &e);

            ps_gradient_add_color_stop(g, 0, &c1);
            ps_gradient_add_color_stop(g, 1, &c2);

            ps_set_source_gradient(gc, g);
            ps_fill(gc);
            ps_gradient_unref(g);
        }

        draw_btnText(gc, U("Ok"), m_commit);
        draw_light(gc, m_commit);
    }

    if (!m_cancel.isEmpty()) {
        ps_rect rc = {m_cancel.x + 2 * b, m_cancel.y + 2 * b, m_cancel.w - 4 * b, m_cancel.h - 4 * b};
        ps_color c = {0.5, 0.5, 0.5, 1};
        ps_set_stroke_color(gc, &c);

        ps_set_line_width(gc, b);
        ps_rounded_rect(gc, &rc, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b);
        ps_stroke(gc);
        ps_set_line_width(gc, 1);

        if (m_btn == 1) {
            ps_rect r2 = {rc.x + b, rc.y + b, rc.w - b, rc.h - b};
            ps_rounded_rect(gc, &r2, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b);

            ps_color c1 = {0, 0, 0, 1};
            ps_color c2 = {1, 0, 0, 1};
            ps_point s = {r2.x, r2.y};
            ps_point e = {r2.x, r2.y + r2.h};
            ps_gradient* g = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s, &e);

            ps_gradient_add_color_stop(g, 0, &c1);
            ps_gradient_add_color_stop(g, 1, &c2);

            ps_set_source_gradient(gc, g);
            ps_fill(gc);
            ps_gradient_unref(g);
        }

        draw_btnText(gc, U("Cancel"), m_cancel);
        draw_light(gc, m_cancel);
    }
}

void InputDialog::OnPaint(ps_context* gc, const Rect* d)
{
    Dialog::OnPaint(gc, d);

    ps_save(gc);

    ps_color c = {1, 1, 1, 1};
    ps_set_text_color(gc, &c);
    ps_font* f = MainWindow::getUIFont();
    ps_font* of = ps_set_font(gc, f);
#ifdef FONT_NO_ANTIALIAS
    ps_set_text_antialias(gc, False);
#else
    if (MainWindow::uiFontAntialias()) {
        ps_set_text_antialias(gc, True);
    } else {
        ps_set_text_antialias(gc, False);
    }
#endif

    draw_msgText(gc, m_textArea);

    draw_btns(gc);

    ps_set_font(gc, of);

    ps_restore(gc);
}

void InputDialog::OnCreate(uint32_t f, int x, int y, int w, int h)
{
    Dialog::OnCreate(f, x, y, w, h);
    int b = TITLE_HEIGHT / 22;
    m_textArea = Rect(5 * b, titleHeight() + 5 * b, w - 10 * b, h - titleHeight() - BUTTON_HEIGHT * 1.5 - 12 * b);

    int btn_width = BUTTON_HEIGHT * 2;
    int border = (width() - BUTTON_HEIGHT * 4) / 4;
    m_commit = Rect(border, h - BUTTON_HEIGHT, btn_width, BUTTON_HEIGHT);
    m_cancel = Rect(width() - border - btn_width, h - BUTTON_HEIGHT, btn_width, BUTTON_HEIGHT);

    m_user->OnCreate(WF_ENABLED | WF_VISIBLE | WF_FOCUSABLE | WF_EDITABLE,
                     b * 5, titleHeight() + b * 4 + m_textArea.h, w - 10 * b, TOOLBAR_HEIGHT / 3 * 2 + 2 * b);

    addChild(m_user);
}

void InputDialog::OnMouseEvent(const MouseEvent* e)
{
    Dialog::OnMouseEvent(e);
    if (e->type() == MouseEvent::MouseDown) {
        if (m_commit.contains(e->x(), e->y())) {
            m_btn = 2;
        } else if (m_cancel.contains(e->x(), e->y())) {
            m_btn = 1;
        } else {
            m_btn = 0;
        }

        if (m_btn) {
            Rect r(0, height() - BUTTON_HEIGHT, width(), BUTTON_HEIGHT);
            Update(&r);
        }
    } else if (e->type() == MouseEvent::MouseUp) {
        if (m_btn) {
            int ret = m_btn;
            m_btn = 0;
            Rect r(0, height() - BUTTON_HEIGHT, width(), BUTTON_HEIGHT);
            Update(&r);
            EndModal(ret);
        }
    }
}

// common dialogs
ustring Dialog::PromptBox(MainWindow* main, const ustring& msg,
                          const ustring& defmsg, const ustring& title)

{
    int width = main->width() / 5 * 4;
    int height = width / 5 * 3;

    InputDialog dlg(0, main);
    dlg.setTitle(title);
    dlg.setMessage(msg);
    dlg.setUserText(defmsg);
    dlg.Create((main->width() - width) / 2, (main->height() - height) / 2, width, height);

    int ret = dlg.Modal();
    if (ret == 1) {
        return ustring();
    } else {
        return dlg.getUserText();
    }
}

//login dialog
class LoginDialog : public Dialog
{
public:
    LoginDialog(Widget* p, MainWindow* m);
    virtual ~LoginDialog();

    ustring getUserName(void) const { return m_user->getText(); }
    ustring getPassWord(void) const { return m_pwd->getText(); }

    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
    virtual void OnMouseEvent(const MouseEvent* e);
private:
    void draw_btns(ps_context* gc);
    void draw_light(ps_context* gc, const Rect& r);
    void draw_btnText(ps_context* gc, const ustring& text, const Rect& r);
    LineEdit* m_user;
    LineEdit* m_pwd;
    Rect m_commit;
    Rect m_cancel;
    int m_btn;
};

LoginDialog::LoginDialog(Widget* p, MainWindow* m)
    : Dialog(p, m)
    , m_user(0)
    , m_pwd(0)
    , m_commit(0, 0, 0, 0)
    , m_cancel(0, 0, 0, 0)
    , m_btn(0)
{
    m_user = new LineEdit(this);
    m_pwd = new LineEdit(this);
}

LoginDialog::~LoginDialog()
{
    delete m_pwd;
    delete m_user;
}

void LoginDialog::draw_btnText(ps_context* gc, const ustring& text, const Rect& r)
{
    if (!text.empty()) {
        ps_size sz = {0};
        ps_get_text_extent(gc, text.c_str(), text.length(), &sz);
        ps_wide_text_out_length(gc, r.x + (r.w - sz.w) / 2, r.y + r.h / 2 - 6 * (TITLE_HEIGHT / 22),
                                (ps_uchar16*)text.c_str(), text.length());
    }
}

void LoginDialog::draw_light(ps_context* gc, const Rect& r)
{
    int b = TITLE_HEIGHT / 22;
    ps_color tc = {1, 1, 1, 0.8};
    ps_color tc2 = {1, 1, 1, 0.2};
    ps_rect lrc = {r.x + 4 * b, r.y + 4 * b, r.w - 7 * b, r.h / 2 - 4 * b};
    ps_rounded_rect(gc, &lrc, 3 * b, 3 * b, 3 * b, 3 * b, 0, 0, 0, 0);
    ps_point s1 = {lrc.x, lrc.y};
    ps_point e1 = {lrc.x, lrc.y + lrc.h};
    ps_gradient* gt = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s1, &e1);
    ps_gradient_add_color_stop(gt, 0, &tc);
    ps_gradient_add_color_stop(gt, 1, &tc2);
    ps_set_source_gradient(gc, gt);
    ps_fill(gc);
    ps_gradient_unref(gt);
}

void LoginDialog::draw_btns(ps_context* gc)
{
    int b = TITLE_HEIGHT / 22;
    if (!m_commit.isEmpty()) {
        ps_rect rc = {m_commit.x + 2 * b, m_commit.y + 2 * b, m_commit.w - 4 * b, m_commit.h - 4 * b};
        ps_color c = {0.5, 0.5, 0.5, 1};
        ps_set_stroke_color(gc, &c);

        ps_set_line_width(gc, b);
        ps_rounded_rect(gc, &rc, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b);
        ps_stroke(gc);
        ps_set_line_width(gc, 1);

        if (m_btn == 2) {
            ps_rect r2 = {rc.x + b, rc.y + b, rc.w - b, rc.h - b};
            ps_rounded_rect(gc, &r2, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b);

            ps_color c1 = {0, 0, 0, 1};
            ps_color c2 = {0.02, 1, 0, 1};
            ps_point s = {r2.x, r2.y};
            ps_point e = {r2.x, r2.y + r2.h};
            ps_gradient* g = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s, &e);

            ps_gradient_add_color_stop(g, 0, &c1);
            ps_gradient_add_color_stop(g, 1, &c2);

            ps_set_source_gradient(gc, g);
            ps_fill(gc);
            ps_gradient_unref(g);
        }

        draw_btnText(gc, U("Ok"), m_commit);
        draw_light(gc, m_commit);
    }

    if (!m_cancel.isEmpty()) {
        ps_rect rc = {m_cancel.x + 2 * b, m_cancel.y + 2 * b, m_cancel.w - 4 * b, m_cancel.h - 4 * b};
        ps_color c = {0.5, 0.5, 0.5, 1};
        ps_set_stroke_color(gc, &c);

        ps_set_line_width(gc, b);
        ps_rounded_rect(gc, &rc, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b);
        ps_stroke(gc);
        ps_set_line_width(gc, 1);

        if (m_btn == 1) {
            ps_rect r2 = {rc.x + b, rc.y + b, rc.w - b, rc.h - b};
            ps_rounded_rect(gc, &r2, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b);

            ps_color c1 = {0, 0, 0, 1};
            ps_color c2 = {1, 0, 0, 1};
            ps_point s = {r2.x, r2.y};
            ps_point e = {r2.x, r2.y + r2.h};
            ps_gradient* g = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s, &e);

            ps_gradient_add_color_stop(g, 0, &c1);
            ps_gradient_add_color_stop(g, 1, &c2);

            ps_set_source_gradient(gc, g);
            ps_fill(gc);
            ps_gradient_unref(g);
        }

        draw_btnText(gc, U("Cancel"), m_cancel);
        draw_light(gc, m_cancel);
    }
}

void LoginDialog::OnPaint(ps_context* gc, const Rect* d)
{
    Dialog::OnPaint(gc, d);

    ps_save(gc);

    ps_color c = {1, 1, 1, 1};
    ps_set_text_color(gc, &c);
    ps_font* f = MainWindow::getUIFont();
    ps_font* of = ps_set_font(gc, f);
#ifdef FONT_NO_ANTIALIAS
    ps_set_text_antialias(gc, False);
#else
    if (MainWindow::uiFontAntialias()) {
        ps_set_text_antialias(gc, True);
    } else {
        ps_set_text_antialias(gc, False);
    }
#endif

    int b = TITLE_HEIGHT / 22;
    ps_wide_text_out_length(gc, b * 5, titleHeight() + b * 8, (ps_uchar16*)U("UserName:"), 9);
    ps_wide_text_out_length(gc, b * 5, titleHeight() + b * 16 + TOOLBAR_HEIGHT / 3 * 2, (ps_uchar16*)U("PassWord:"), 9);

    draw_btns(gc);

    ps_set_font(gc, of);

    ps_restore(gc);
}

void LoginDialog::OnMouseEvent(const MouseEvent* e)
{
    Dialog::OnMouseEvent(e);
    if (e->type() == MouseEvent::MouseDown) {
        if (m_commit.contains(e->x(), e->y())) {
            m_btn = 2;
        } else if (m_cancel.contains(e->x(), e->y())) {
            m_btn = 1;
        } else {
            m_btn = 0;
        }

        if (m_btn) {
            Rect r(0, height() - BUTTON_HEIGHT, width(), BUTTON_HEIGHT);
            Update(&r);
        }
    } else if (e->type() == MouseEvent::MouseUp) {
        if (m_btn) {
            int ret = m_btn;
            m_btn = 0;
            Rect r(0, height() - BUTTON_HEIGHT, width(), BUTTON_HEIGHT);
            Update(&r);
            EndModal(ret);
        }
    }
}

void LoginDialog::OnCreate(uint32_t f, int x, int y, int w, int h)
{
    Dialog::OnCreate(f, x, y, w, h);
    int b = TITLE_HEIGHT / 22;
    int btn_width = BUTTON_HEIGHT * 2;
    int border = (width() - BUTTON_HEIGHT * 4) / 4;
    m_commit = Rect(border, h - BUTTON_HEIGHT, btn_width, BUTTON_HEIGHT);
    m_cancel = Rect(width() - border - btn_width, h - BUTTON_HEIGHT, btn_width, BUTTON_HEIGHT);

    m_user->OnCreate(WF_ENABLED | WF_VISIBLE | WF_FOCUSABLE | WF_EDITABLE,
                     b * 45, titleHeight() + b * 4, w - 50 * b, TOOLBAR_HEIGHT / 3 * 2 + 2 * b);

    m_pwd->OnCreate(WF_ENABLED | WF_VISIBLE | WF_FOCUSABLE | WF_EDITABLE,
                    b * 45, titleHeight() + b * 10 + TOOLBAR_HEIGHT / 3 * 2, w - 50 * b, TOOLBAR_HEIGHT / 3 * 2 + 2 * b);

    m_pwd->setPassWordMode(true);

    addChild(m_user);
    addChild(m_pwd);
}

//common dialogs
bool Dialog::LoginBox(MainWindow* main, ustring& user, ustring& pwd, const ustring& title)
{
    int width = main->width() / 5 * 4;
    int height = width / 5 * 3;

    LoginDialog dlg(0, main);
    dlg.setTitle(title);
    dlg.Create((main->width() - width) / 2, (main->height() - height) / 2, width, height);

    int ret = dlg.Modal();
    if (ret == 1) {
        return false;
    } else {
        user = dlg.getUserName();
        pwd = dlg.getPassWord();
        return true;
    }
}

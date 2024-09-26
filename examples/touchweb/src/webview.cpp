/* webview.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */
#include "config.h"
#include <string.h>
#include <picasso/picasso.h>

#include "url.h"
#include "macross.h"
#include "webview.h"
#include "tabpage.h"
#include "dialog.h"
#include "unicode.h"
#include "popmenu.h"
#include "topmost.h"
#include "history.h"
#include "network.h"
#include "fillmgr.h"
#include "addressbar.h"
#include "mainwindow.h"
#include "contextmenu.h"
#include "application.h"
#include "extracfg.h"
#include "scripts.h"
#include "autofill.h"
#include "renderthread.h"

#if defined(WIN32)
    #include "application_win32.h"
    #include "webview_win32.h"
#endif

#ifdef GTK2
    #include "application_gtk2.h"
    #include "webview_gtk2.h"
#endif

#define BUFFER_RAISE 1  //define buffer raise mode.

#if BUFFER_RAISE
/*
 * we need create a 2x3 buffer.
 * Note: I never want to modify the code, because only the alien can understand.
 */
#define BUFFER_WIDTH(w) (w*2)
#define BUFFER_HEIGHT(h) (h*3)
#define START_X(w) (0)
#define START_Y(h) (0)
#define CENTER_X(w) (w>>1)
#define CENTER_Y(h) (h)
#define END_X(w) (w)
#define END_Y(h) (h<<1)
static int start_offset_X(MaCrossView* v, int currentX, int width)
{
    MC_SIZE size = {0, 0};
    int startX = 0;

    macross_view_get_contents_size(v, &size);
    int last = (size.w - currentX - width) > 0 ? (size.w - currentX - width) : 0;
    currentX = currentX < 0 ? 0 : currentX;
    //offset x
    if (currentX < (width >> 1)) {
        startX = START_X(width) + currentX;
    } else if ((currentX > width) && (last < (width >> 1)) && (last >= 0)) {
        startX = END_X(width) - last;
    } else if ((currentX > (width >> 1)) && (last < (width >> 1)) && (last >= 0)) {
        /*
         * Note: This case because content width < buffer's width (height*2), so use center X + offset
         *       to display the right contents. : )  (too difficult!!)
         */
        int offset = currentX + (width >> 1) - (width);
        startX = CENTER_X(width) + offset;
    } else {
        startX = CENTER_X(width);
    }
    return startX;
}

static int start_offset_Y(MaCrossView* v, int currentY, int height)
{
    MC_SIZE size = {0, 0};
    int startY = 0;

    macross_view_get_contents_size(v, &size);
    int last = (size.h - currentY - height) > 0 ? (size.h - currentY - height) : 0;
    currentY = currentY < 0 ? 0 : currentY;
    //offset y
    if (currentY < height) {
        startY = START_Y(height) + currentY;
    } else if ((currentY > (height << 1)) && (last < height) && (last >= 0)) {
        startY = END_Y(height) - last;
    } else if ((currentY > height) && (last < height) && (last >= 0)) { //special size!
        /*
         * Note: This case because content height < buffer's height (height*3), so use center Y + offset
         *       to display the right contents. : )  (too difficult!!)
         */
        int offset = currentY + height - (height << 1);
        startY = CENTER_Y(height) + offset;
    } else {
        startY = CENTER_Y(height);
    }
    return startY;
}
#else
#define BUFFER_WIDTH(w) (w)
#define BUFFER_HEIGHT(h) (h)
#define start_offset_X(v, cx, w) (0)
#define start_offset_Y(v, cy, h) (0)
#endif

#if defined(MOBILE_PHONE)
    #define DEFAULE_SCALE   2
#else
    #define DEFAULE_SCALE   GLOBAL_SCALE
#endif

//webview
class WebViewImpl
{
public:
    WebViewImpl()
        : view(0), main(0), width(0), height(0), plat(0), mode(0), scale(1.0f * DEFAULE_SCALE)
        , scrollX(0), scrollY(0), vposX(0), vposY(0), voffsetX(0), voffsetY(0), oldContentX(0), oldContentY(0)
        , capture(false), indrag(false), mouseDown(false), fullmode(false)
        , mouseX(0), mouseY(0), startX(0), startY(0)
        , ticks(0), time(1), tickets(0), drx(0), dry(0), scx(0), scy(0), newurl(true), loading(0)
    {
        resetTicket();
    }

    ~WebViewImpl()
    {
    }

    void resetTicket(void)
    {
        tickets = Application::tickCount();
    }

    unsigned long ticket(void)
    {
        return ((Application::tickCount() - tickets) >> 6) + 1;
    }

    ustring title;
    ustring location;
    MaCrossView* view;
    MainWindow* main;
    int width;
    int height;
    WebViewPlat* plat;
    int mode;
    float scale;
    int scrollX;
    int scrollY;
    int vposX; //page current position
    int vposY;
    int voffsetX; //page start offset
    int voffsetY;
    int oldContentX;
    int oldContentY;
    //mouse event helper
    bool capture;
    bool indrag;
    bool mouseDown;
    bool fullmode;
    int mouseX;
    int mouseY;
    int startX;
    int startY;
    volatile int ticks;
    volatile int time;
    unsigned long tickets;
    int drx;
    int dry;
    int scx;
    int scy;
    //progress
    bool newurl;
    volatile unsigned int loading;
};

WebView::WebView(Widget* parent)
    : Widget(parent)
    , m_impl(new WebViewImpl)
{
    m_impl->plat = new WebViewPlat(this);
}

WebView::~WebView()
{
    delete m_impl->plat;
    delete m_impl;
}

static void dirty_rect(MaCrossView* view, const MC_RECT* r)
{
    WebView* web = (WebView*)macross_view_additional_data(view);
    Rect rc(r->x, r->y, r->w, r->h);
    web->Update(&rc);
}

static void update(MaCrossView* view)
{
    WebView* web = (WebView*)macross_view_additional_data(view);
    web->Update(NULL);
}

static void position(MaCrossView* view, int ox, int oy, int nx, int ny)
{
    WebView* web = (WebView*)macross_view_additional_data(view);
    web->savePosition(ox, oy);
}

static void set_title(MaCrossView* view, const char* title)
{
    WebView* web = (WebView*)macross_view_additional_data(view);
    web->setTitle(Unicode::ConvertUTF8ToUTF16(std::string(title)));
}

static void set_location(MaCrossView* view, const char* location)
{
    WebView* web = (WebView*)macross_view_additional_data(view);
    web->setLocation(Unicode::ConvertUTF8ToUTF16(std::string(location)));
}

static MaCrossView* open_window(const char* url, unsigned int f, const MC_RECT* rc)
{
    TabPage* p = Application::getInstance()->getMainWindow()->getTabs();

    if (p->isFullMode()) {
        WebView* v = p->getActiveView();
        v->loadUrl(url);

        return v->impl()->view;
    } else {
        if (!p->newView(url)) {
            return 0;
        }

        WebView* v = p->getActiveView();
        return v->impl()->view;
    }
}

static void set_focus(MaCrossView* view)
{
    WebView* web = (WebView*)macross_view_additional_data(view);
    web->setFocus(true);
}

static void kill_focus(MaCrossView* view)
{
    WebView* web = (WebView*)macross_view_additional_data(view);
    web->setFocus(false);
}

//calc offset which r1 contians r2 needed.
static void offsets_combine_rects(const Rect& from, const Rect& to, int* x, int* y)
{
    if (to.x < from.x) {
        *x += to.x - from.x;
    } else if ((to.x + to.w) > (from.x + from.w)) {
        *x += (to.x + to.w) - (from.x + from.w);
    } else {
        *x += 0;
    }

    if (to.y < from.y) {
        *y += to.y - from.y;
    } else if ((to.y + to.h) > (from.y + from.h)) {
        *y += (to.y + to.h) - (from.y + from.h);
    } else {
        *y += 0;
    }
}

static Rect compute_rect(const Rect& r)
{
    int x = r.x * GLOBAL_SCALE;
    int y = r.y * GLOBAL_SCALE;
    int w = r.w * GLOBAL_SCALE;
    int h = r.h * GLOBAL_SCALE;
    return Rect(x, y, w, h);
}

static void change_focus(MaCrossView* view, const MC_RECT* rc)
{
    WebView* web = (WebView*)macross_view_additional_data(view);
    Rect prc = web->getViewportRect();
    Rect crc = web->getContentsRect();
    Rect frc = compute_rect(Rect(rc->x, rc->y, rc->w, rc->h));

    if (!prc.contents(frc)) {
        frc.intersect(crc);
        int px = 0, py = 0;
        offsets_combine_rects(prc, frc, &px, &py);

        px += crc.x - web->getLastPosX();
        py += crc.y - web->getLastPosY();

        web->setViewScroll(-px, -py);
    }
}

static void start_layout(MaCrossView* view)
{
    WebView* web = (WebView*)macross_view_additional_data(view);
    web->resetPage();
}

static void context_menu(MaCrossView* view, const MC_CONTEXT_MENU_DATA* menu)
{
    int b = TOOLBAR_HEIGHT / 30;
    int height = 0;
    WebView* web = (WebView*)macross_view_additional_data(view);
    ContextMenu* p = Application::getInstance()->getMainWindow()->getContextMenu();
    p->clear();
    p->setX(p->vx() - web->impl()->voffsetX);
    p->setY((p->vy() + Application::getInstance()->getMainWindow()->getTabs()->y()) - web->impl()->voffsetY);
    p->setWidth(104 * b);

    bool full = Application::getInstance()->getMainWindow()->getTabs()->isFullMode();

    if (menu->type & MENU_TYPE_LINK) {
        if (full) {
            height += TOOLBAR_HEIGHT;
        } else {
            height += TOOLBAR_HEIGHT * 3;
        }
        p->setLinkText(std::string(menu->link_url));
    }

    if (menu->type & MENU_TYPE_IMAGE) {
        if (full) {
            height += TOOLBAR_HEIGHT;
        } else {
            height += TOOLBAR_HEIGHT * 3;
        }
        p->setImgLinkText(std::string(menu->image_url));
    }

    if (full) {
        p->setWidth(80 * b);
        p->setEasyMode(true);
    } else {
        p->setEasyMode(false);
    }

    p->setHeight(height);
    p->create();
    p->Show();
}

static void error_report(int e, const char* url)
{
    MainWindow* m = Application::getInstance()->getMainWindow();
    if ((e > 400000) && (e < 400005)) {
        Dialog::AlertBox(m, U("ÎÞ·¨·ÃÎÊ¸ÃµØÖ·"), U("¾¯¸æ"));
    } else if ((e > 401000) && (e < 402000 )) {
        Dialog::AlertBox(m, U("ÍøÂç´íÎó"), U("¾¯¸æ"));
    }
}

static void loading_progress(MaCrossView* view, unsigned int prog, MC_BOOL finish)
{
    WebView* web = (WebView*)macross_view_additional_data(view);
    web->setLoading(prog, finish ? true : false);
}

static void alert_box(MaCrossView* view, const char* msg)
{
    MainWindow* m = Application::getInstance()->getMainWindow();
    Dialog::AlertBox(m, Unicode::ConvertUTF8ToUTF16(msg), U("¾¯¸æ"));
}

static MC_BOOL confirm_box(MaCrossView* view, const char* msg)
{
    MainWindow* m = Application::getInstance()->getMainWindow();
    if (Dialog::ConfirmBox(m, Unicode::ConvertUTF8ToUTF16(msg), U("ÐÅÏ¢"))) {
        return True;
    } else {
        return False;
    }
}

static char* prompt_box(MaCrossView* view, const char* msg, const char* def)
{
    static std::string ret;

    MainWindow* m = Application::getInstance()->getMainWindow();
    ret = Unicode::ConvertUTF16ToUTF8(Dialog::PromptBox(m, Unicode::ConvertUTF8ToUTF16(msg),
                                                        Unicode::ConvertUTF8ToUTF16(def), U("ÐÅÏ¢")));
    if (ret.empty()) {
        return 0;
    } else {
        return const_cast<char*>(ret.c_str());
    }
}

static MC_BOOL provide_auth(const char* title, char* user, unsigned int us, char* pwd, unsigned int ps)
{
    MainWindow* m = Application::getInstance()->getMainWindow();
    ustring uname;
    ustring pass;
    if (Dialog::LoginBox(m, uname, pass, Unicode::ConvertUTF8ToUTF16(title))) {
        strncpy(user, Unicode::ConvertUTF16ToUTF8(uname).c_str(), us - 1);
        strncpy(pwd, Unicode::ConvertUTF16ToUTF8(pass).c_str(), ps - 1);
        return True;
    } else {
        return False;
    }
}

static MC_BOOL network_request(const char* url)
{
    if ((strncasecmp("file://", url, 7) != 0) && !Application::getInstance()->getNetService()->networkCheck()) {
        //error report
        return FALSE;
    }
    return TRUE;
}

static char* user_agent_custom(const char* url)
{
    if (Application::getInstance()->getNetService()->needProxy()) {
        return "Mozilla/4.0 (compitable; msie 4.0; Windows CE; PPC)";
    } else {
        return NULL;
    }
}

static void ime_status(MC_BOOL enable)
{
    SetImeStatus(enable ? true : false);
}

static MC_BOOL submit_form(const char* base_url, const char* form_name, const char* url,
                           const char* name, const char* value, MC_BOOL is_end)
{
    AutoFill* pa = Application::getInstance()->getAutofill();
    if (!pa) {
        return FALSE;
    }

    AutoFillManager* pm = Application::getInstance()->getMainWindow()->getAutoFill();
    if (!pm || pm->isVisible()) {
        return FALSE;
    }

    AutoFill::FormData& form = pm->getOldForm();

    if (!pm->isFinded()) {
        pm->setFinded(true);
        if (pa->findFormData(url_strip_query(base_url), form)) {
            if (form.notsave) {
                return FALSE; // don't save this form.
            }
        }
    }

    if (form.notsave) {
        return FALSE;
    }

    AutoFill::FormData& newForm = pm->getNewForm();

    if (newForm.url.empty()) {
        newForm.url = url_strip_query(base_url);
    }

    if (newForm.formid.empty()) {
        newForm.formid = std::string(form_name);
    }

    if (newForm.action.empty()) {
        newForm.action = url_strip_query(url);
    }

    newForm.values[std::string(name)] = std::string(value);

    if (is_end) {
        if (form == newForm) {
            pm->clear();
        } else {
            pm->Show();
        }
    }
    return TRUE;
}

static MC_BOOL choose_file(char* utf8_file_name, unsigned int buffer_size, MC_BOOL is_save)
{
    //FIXME: not support download manager.
    if (is_save) {
        return FALSE;
    }

    uchar_t name_buffer[PATHMAX] = {0};
    MainWindow* main = Application::getInstance()->getMainWindow();

    if (main && main->getChooseFile(name_buffer, PATHMAX)) {
        //copy filename.
        std::string name = Unicode::ConvertUTF16ToUTF8(ustring(name_buffer, PATHMAX));
        strncpy(utf8_file_name, name.c_str(), buffer_size - 1);
        return TRUE;
    }
    return FALSE;
}

void WebView::setPlatformView(MaCrossView* view)
{
    m_impl->view = view;
}

MaCrossView* WebView::getPlatformView(void) const
{
    return m_impl->view;
}

RenderThread* WebView::render = NULL;

void WebView::init(void)
{
    static bool initialize = false;
    if (!initialize) {
        // init webview for global
        MC_CALLBACK_INFO info;
        memset(&info, 0, sizeof(MC_CALLBACK_INFO));

        info.cb_invalidate_rect = dirty_rect;
        info.cb_update_view_now = update;
        info.cb_position_changed = position;
        info.cb_set_title = set_title;
        info.cb_set_location = set_location;
        info.cb_open_window = open_window;
        info.cb_context_menu = context_menu;
        info.cb_set_focus = set_focus;
        info.cb_kill_focus = kill_focus;
        info.cb_change_focus = change_focus;
        info.cb_start_layout = start_layout;
        info.cb_error = error_report;
        info.cb_loading_progress = loading_progress;
        info.cb_alert_box = alert_box;
        info.cb_confirm_box = confirm_box;
        info.cb_prompt_box = prompt_box;
        info.cb_provide_auth = provide_auth;
        info.cb_set_ime_enable = ime_status;
        info.cb_allow_main_request = network_request;
        info.cb_user_agent_custom = user_agent_custom;
        info.cb_save_submit_form = submit_form;
        info.cb_choose_file = choose_file;

        info.cb_create_popup_menu = CreatePopMenu;
        info.cb_destroy_popup_menu = DestroyPopMenu;

        macross_set_callback(&info);
        initialize = true;

#if THREAD_RENDER
        // initialize render thread loop.
        WebView::render = new RenderThread;
        // FIXME: thread sync event loop here init.

        WebView::render->Start();
#endif
    }
}

void WebView::shutdown(void)
{
#if THREAD_RENDER
    WebView::render->Exit();
    int i = 0;
    while (WebView::render->isRunning()) {
        i++;
        vgcl::VThread::Sleep(10); // wait for rendering thread exit.
    }
    delete WebView::render;
    WebView::render = NULL;
#endif
}

void WebView::Update(const Rect* r)
{
    if (m_impl->main->getTabs()->isActiveView(this)) {
        Rect rc;
        if (r) {
            rc = *r;
            rc.x -= m_impl->voffsetX;
            rc.y -= m_impl->voffsetY;
            Widget::Update(&rc);
        } else {
            Widget::Update(r);
        }

#if THREAD_RENDER
        if (!m_impl->indrag) {
            WebEventParam param;
            param.cmd.view = m_impl->view;
            param.cmd.dirty = rc;
            WebEvent event(this, WebEvent::PaintView, WebEvent::HighLevel, param);
            WebView::render->sendEvent(event);
        }
#endif
    }

}

void WebView::OnChar(uchar_t code)
{
    Widget::OnChar(code);

    std::string str = Unicode::ConvertUTF16ToUTF8(ustring(&code, 1));
    macross_view_input_text(m_impl->view, str.data());
}

void WebView::OnSetFocus(void)
{
    Widget::OnSetFocus();
    macross_view_set_focus(m_impl->view);
}

void WebView::OnKillFocus(void)
{
    Widget::OnKillFocus();
    macross_view_kill_focus(m_impl->view);
}

void WebView::OnPaint(ps_context* gc, const Rect* r)
{
    Widget::OnPaint(gc, r);
    if (!m_impl->indrag) {
#if !defined(THREAD_RENDER)
        macross_view_update(m_impl->view, 0);
#endif
    }

    int xscroll = m_impl->scrollX - m_impl->voffsetX;
    int yscroll = m_impl->scrollY - m_impl->voffsetY;

    m_impl->plat->Paint(m_impl->main->platform(), xscroll, yscroll, r->x, r->y, r->w, r->h);

    // if (m_impl->indrag)
    // paint_scroll_bars();
}

void WebView::OnCreate(uint32_t flags, int x, int y, int w, int h)
{
    Widget::OnCreate(flags, x, y, w, h);
    m_impl->width = w;
    m_impl->height = h;

    int real_width = BUFFER_WIDTH(w);
    int real_height = BUFFER_HEIGHT(h);
    m_impl->plat->CreateStoreBuffer(real_width, real_height);
    int pitch = 0;
    unsigned char* buffer = m_impl->plat->GetBuffer(&pitch);

#if THREAD_RENDER
    WebEventParam param;
    param.desc.width = w;
    param.desc.height = h;
    param.desc.buffer = buffer;
    param.desc.buffer_width = real_width;
    param.desc.buffer_height = real_height;
    param.desc.buffer_pitch = pitch;
    param.desc.default_scale = m_impl->scale;

    WebEvent event(this, WebEvent::CreateView, WebEvent::HighLevel, param);
    WebView::render->sendEvent(event);
#else
    MC_SIZE lsize = {w, h};
    m_impl->view = macross_view_create(buffer, real_width, real_height, pitch, this);
    macross_view_set_minimum_layout_size(m_impl->view, &lsize);
    macross_view_set_scale_factor(m_impl->view, (int)(100 * m_impl->scale));
#endif
}

void WebView::setSize(int w, int h)
{
    setWidth(w);
    setHeight(h);
}

float WebView::zoomFactor(void) const
{
    return m_impl->scale;
}

void WebView::setZoomFactor(float scale)
{
    if (m_impl->scale != scale) {
        macross_view_set_scale_factor(m_impl->view, (int)(100 * scale));
        m_impl->scale = scale;
        if (renderMode() == Render_SmartFit) {
            setRenderMode(Render_SmartFit); // reset smart fit
        }
        //set new positions
        m_impl->vposX = 0;
        m_impl->vposY = 0;
        m_impl->oldContentX = 0;
        m_impl->oldContentY = 0;
        m_impl->voffsetX = start_offset_X(m_impl->view, m_impl->vposX, m_impl->width);
        m_impl->voffsetY = start_offset_Y(m_impl->view, m_impl->vposY, m_impl->height);
        clear_paint_page();
        macross_view_set_position(m_impl->view, m_impl->vposX - m_impl->voffsetX, m_impl->vposY - m_impl->voffsetY);
    }
}

void WebView::update_scroll_view_task(void* p)
{
    WebViewImpl* impl = (WebViewImpl*)p;

    impl->indrag = false;
    impl->vposX -= impl->scrollX;
    impl->vposY -= impl->scrollY;

    impl->voffsetX = start_offset_X(impl->view, impl->vposX, impl->width);
    impl->voffsetY = start_offset_Y(impl->view, impl->vposY, impl->height);

    macross_view_set_position(impl->view, impl->vposX - impl->voffsetX, impl->vposY - impl->voffsetY);
    impl->scrollX = 0;
    impl->scrollY = 0;
}

void WebView::update_scroll_view(void)
{
    Widget::postEvent(this, EVENT_FUNC(WebView, update_scroll_view_task), EVENT_PARAM(m_impl));
}

void WebView::OnIdle(void)
{
    Widget::OnIdle();
    m_impl->ticks++;
    if (m_impl->ticks > 20) {
        m_impl->ticks = 0;
        m_impl->time++;
    }

    if (m_impl->scx || m_impl->scy) {
        int b = TOOLBAR_HEIGHT / 30;

        if (m_impl->scx) { ABS_DEC(m_impl->scx, 10 * b); }
        if (m_impl->scy) { ABS_DEC(m_impl->scy, 10 * b); }

        if ((ABS(m_impl->scx) < 2) && (ABS(m_impl->scy) < 2)) {
            m_impl->scx = 0;
            m_impl->scy = 0;
            update_scroll_view();
        } else {
            int xoffset = m_impl->scx / 5;
            int yoffset = m_impl->scy / 5;

            if (!m_impl->mode) {
                xoffset = 0;
            }

            if (!adjust_scroll_increase(m_impl->scrollX, xoffset, m_impl->scrollY, yoffset)) {
                m_impl->scx = 0;
                m_impl->scy = 0;
                update_scroll_view();
            } else {
                m_impl->indrag = true;
                Update(NULL);
            }
        }
    }

    if (m_impl->mouseDown) {
        m_impl->mouseDown = false;

        MC_MOUSE_EVENT evt;
        evt.button = MOUSE_BTN_LEFT;
        evt.type = EVT_MOUSE_UP;
        evt.point.x = m_impl->mouseX;
        evt.point.y = m_impl->mouseY;
        evt.modifier = 0;
        macross_mouse_event(m_impl->view, &evt);
    }

    if (m_impl->capture && !(m_impl->indrag) && (m_impl->ticket() > 10)) {
        m_impl->capture = false;

        MC_CONTEXT_EVENT evt;
        evt.point.x = m_impl->startX;
        evt.point.y = m_impl->startY;
        Application::getInstance()->getMainWindow()->getContextMenu()->setValX(m_impl->startX);
        Application::getInstance()->getMainWindow()->getContextMenu()->setValY(m_impl->startY);
        evt.modifier = 0;
        macross_context_menu_event(m_impl->view, &evt);
    }
}

void WebView::OnDestroy(void)
{
    Widget::OnDestroy();

#if THREAD_RENDER
    WebEventParam param;
    param.cmd.view = m_impl->view;
    WebEvent event(this, WebEvent::DestroyView, WebEvent::HighLevel, param);
    WebView::render->sendEvent(event);
#else
    macross_view_destroy(m_impl->view);
#endif
    m_impl->plat->DestroyStoreBuffer();
}

void WebView::OnKeyEvent(const KeyEvent* e)
{
    Widget::OnKeyEvent(e);

    MC_KEY_EVENT evt;
    if (e->value() >= KEY_LEFT && e->value() <= KEY_DOWN) {
        switch (e->value()) {
            case KEY_LEFT: {
                    evt.type = (MC_KEY_EVENT_TYPE)e->type();
                    evt.key = KEY_TAB;
                    evt.modifier = MF_SHIFT;
                    macross_keyboard_event(m_impl->view, &evt);
                }
                break;
            case KEY_RIGHT: {
                    evt.type = (MC_KEY_EVENT_TYPE)e->type();
                    evt.key = KEY_TAB;
                    evt.modifier = 0;
                    macross_keyboard_event(m_impl->view, &evt);
                }
                break;
            case KEY_UP: {
                    int len = 20 * GLOBAL_SCALE;
                    if (e->type() == KeyEvent::KeyDown) {
                        MouseEvent e1(1, 1, 0, len);
                        OnMouseEvent(&e1);

                        MouseEvent e2(2, 1, 0, 0);
                        OnMouseEvent(&e2);

                        MouseEvent e3(0, 1, 0, 0);
                        OnMouseEvent(&e3);
                    }
                }
                break;
            case KEY_DOWN: {
                    int len = 20 * GLOBAL_SCALE;
                    if (e->type() == KeyEvent::KeyDown) {
                        MouseEvent e1(1, 1, 0, 0);
                        OnMouseEvent(&e1);

                        MouseEvent e2(2, 1, 0, len);
                        OnMouseEvent(&e2);

                        MouseEvent e3(0, 1, 0, len);
                        OnMouseEvent(&e3);
                    }
                }
                break;
        }
    } else {
        evt.type = (MC_KEY_EVENT_TYPE)e->type();
        evt.key = (MC_VIRTUAL_KEY)e->value();
        evt.modifier = 0;
        macross_keyboard_event(m_impl->view, &evt);
    }
}

void WebView::OnMouseEvent(const MouseEvent* e)
{
    Widget::OnMouseEvent(e);

    if (e->type() == MouseEvent::MouseDown) {
        Application::getInstance()->getMainWindow()->getContextMenu()->Hide();
        m_impl->drx = e->x();
        m_impl->dry = e->y();
        m_impl->time = 1;
        m_impl->resetTicket();
        m_impl->startX = e->x() + m_impl->voffsetX;
        m_impl->startY = e->y() + m_impl->voffsetY;
        m_impl->capture = true;
    } else if (e->type() == MouseEvent::MouseUp) {
        if (m_impl->indrag) {
            m_impl->indrag = false;
            int b = TOOLBAR_HEIGHT / 30;

            int xs = (e->x() - m_impl->drx) / m_impl->time;
            int ys = (e->y() - m_impl->dry) / m_impl->time;

            if (xs) {
                int vxs = (ABS(xs) - 50 * b > 0) ? (ABS(xs) - 50 * b) : 0;
                m_impl->scx = xs / ABS(xs) * vxs;
            } else {
                m_impl->scx = 0;
            }

            if (ys) {
                int vys = (ABS(ys) - 50 * b > 0) ? (ABS(ys) - 50 * b) : 0;
                m_impl->scy = ys / ABS(ys) * vys;
            } else {
                m_impl->scy = 0;
            }
            //clear the scrollbars
            // clear_scroll_bars();
            if ((ABS(m_impl->scx) < 2) && (ABS(m_impl->scy) < 2)) {
                m_impl->scx = 0;
                m_impl->scy = 0;
                update_scroll_view();
            }
        } else {
            if (m_impl->ticket() < 10) {
                MC_MOUSE_EVENT evt;
                evt.point.x = m_impl->startX;
                evt.point.y = m_impl->startY;
                evt.button = MOUSE_BTN_LEFT;
                evt.type = EVT_MOUSE_DOWN;
                evt.modifier = 0;
                macross_mouse_event(m_impl->view, &evt);
                m_impl->mouseDown = true;
                m_impl->mouseX = m_impl->startX;
                m_impl->mouseY = m_impl->startY;
            }
        }

        m_impl->drx = 0;
        m_impl->dry = 0;
        m_impl->time = 1;
        m_impl->resetTicket();
        m_impl->startX = 0;
        m_impl->startY = 0;
        m_impl->capture = false;
    } else if (e->type() == MouseEvent::MouseMove) {
        if (m_impl->capture) {
            int xpos = e->x() + m_impl->voffsetX;
            int ypos = e->y() + m_impl->voffsetY;

            int offsetX = m_impl->startX - xpos;
            int offsetY = m_impl->startY - ypos;
            if (ABS(offsetX) > MAX_DRAG_STEP || ABS(offsetY) > MAX_DRAG_STEP || m_impl->indrag) {
                if (m_impl->mode) {
                    offsetX = 0;
                }

                if (adjust_scroll_increase(m_impl->scrollX, -offsetX, m_impl->scrollY, -offsetY)) {
                    Update(NULL);
                }

                //clear_scroll_bars();
                m_impl->indrag = true;
                m_impl->startX = e->x() + m_impl->voffsetX;
                m_impl->startY = e->y() + m_impl->voffsetY;
            }
        }
    }
}

bool WebView::adjust_scroll_increase(int& x, int xf, int& y, int yf)
{
    int ox = x;
    int oy = y;
    MC_SIZE size = {0, 0};
    int px = m_impl->vposX;
    int py = m_impl->vposY;
    macross_view_get_contents_size(m_impl->view, &size);
    int tx = x + xf;
    int ty = y + yf;

    if (tx > 0) {
        x = tx > px ? px : tx;
    } else if (tx < 0) {
        x = (px - tx + width()) > size.w ? x : tx;
    }

    if (ty > 0) {
        y = ty > py ? py : ty;
    } else if (ty < 0) {
        y = (py - ty + height()) > size.h ? y : ty;
    }

    return !((x == ox) && (y == oy)); //validate scroll
}

void WebView::paint_scroll_bars(void)
{
    ps_context* gc = m_impl->plat->getContext();

    int b = TOOLBAR_HEIGHT / 30;

    ps_rect r = {b, height() - 6 * b, width() - 7 * b, 5 * b};
    ps_rect r2 = {width() - 6 * b, b, 5 * b, height() - 7 * b};

    ps_color c = {0.2, 0.2, 0.2, 0.5};
    ps_set_source_color(gc, &c);
    if (!m_impl->mode) {
        ps_rounded_rect(gc, &r, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b);
    }
    ps_rounded_rect(gc, &r2, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b);
    ps_fill(gc);

    MC_SIZE psz = {0, 0};
    macross_view_get_contents_size(m_impl->view, &psz);
    int px = 0, py = 0;
    macross_view_get_position(m_impl->view, &px, &py);
    int vl = 0, hl = 0, vp = 0, hp = 0;
    if (psz.w && psz.h) {
        vl = (height() - 8 * b) * height() / psz.h;
        hl = (width() - 8 * b) * width() / psz.w;
        vp = height() * py / psz.h;
        hp = width() * px / psz.w;
    } else {
        vl = 30 * b;
        hl = 30 * b;
        vp = 0;
        hp = 0;
    }

    if (vl < 16 * b) { vl = 16 * b; }
    if (hl < 16 * b) { hl = 16 * b; }

    if ((vp + vl) > (height() - 7 * b)) { vp = height() - vl - 5 * b; }
    if ((hp + hl) > (width() - 7 * b)) { hp = width() - hl - 5 * b; }

    ps_color cb = {0.1, 0.1, 0.1, 1};
    ps_set_source_color(gc, &cb);
    ps_rect r3 = {hp + 2 * b, height() - 5 * b, hl - 4 * b, 3 * b};
    ps_rect r4 = {width() - 5 * b, vp + 2 * b, 3 * b, vl - 4 * b};
    if (!m_impl->mode) {
        ps_rounded_rect(gc, &r3, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b);
    }
    ps_rounded_rect(gc, &r4, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b, 3 * b);
    ps_fill(gc);
}

void WebView::clear_scroll_bars(void)
{
    int b = TOOLBAR_HEIGHT / 30;
    MC_RECT rv = {width() - 8 * b, 0, 8 * b, height()};
    Rect rvs(rv.x, rv.y, rv.w, rv.h);
    MC_RECT rh = {0, height() - 8 * b, width(), 8 * b};
    Rect rhs(rh.x, rh.y, rh.w, rh.h);
    Update(&rvs);
    if (!m_impl->mode) {
        Update(&rhs);
    }
}

const ustring& WebView::title(void) const
{
    return m_impl->title;
}

const ustring& WebView::location(void) const
{
    return m_impl->location;
}

void WebView::setTitle(const ustring& title)
{
    m_impl->title = title;
}

void WebView::setLocation(const ustring& location)
{
    m_impl->location = location;

    m_impl->main->getTabs()->locationChanged(this);
}

int WebView::renderMode(void) const
{
    MC_RENDERING_MODE mode;
    macross_view_get_rendering_mode(m_impl->view, &mode);
    return (int)mode;
}

void WebView::setRenderMode(int m)
{
    m_impl->mode = m;
    MC_RENDERING_MODE mode = (MC_RENDERING_MODE)m;
    macross_view_set_rendering_mode(m_impl->view, mode, (int)(width() / m_impl->scale));

    if (m_impl->mode) {
        m_impl->vposX = 0;
        m_impl->vposY = 0;
        m_impl->voffsetX = 0;
        m_impl->voffsetY = 0;
        m_impl->oldContentX = 0;
        m_impl->oldContentY = 0;
        macross_view_set_position(m_impl->view, 0, 0);
    }
}

void WebView::setMainWindow(MainWindow* main)
{
    m_impl->main = main;
}

WebViewImpl* WebView::impl(void) const
{
    return m_impl;
}

void WebView::loadUrl(const std::string& url)
{
    setLocation(Unicode::ConvertUTF8ToUTF16(url));
#if THREAD_RENDER
    WebEventParam param;
    param.cmd.view = m_impl->view;
    param.cmd.url = url;
    WebEvent event(this, WebEvent::LoadUrl, WebEvent::DefaultLevel, param);
    WebView::render->sendEvent(event);
#else
    macross_view_open_url(m_impl->view, url.c_str());
#endif
}

void WebView::loadHtml(const std::string& string, const std::string& refurl)
{
    macross_view_load_html(m_impl->view, string.c_str(), refurl.c_str());
}

void WebView::stopLoad(void)
{
    macross_view_stop(m_impl->view);
}

void WebView::goBack(void)
{
    macross_view_backward(m_impl->view);
}

void WebView::goForward(void)
{
    macross_view_forward(m_impl->view);
}

void WebView::reLoad(void)
{
    if (!m_impl->location.empty()) {
        macross_view_reload(m_impl->view);
    }
}

void WebView::reFresh(void)
{
    macross_view_repaint(m_impl->view);
}

void WebView::reLayout(void)
{
    macross_view_reflow(m_impl->view);
}

void WebView::clear_paint_page(void)
{
    ps_context* gc = m_impl->plat->getContext();
    ps_color c = {1, 1, 1, 1};
    ps_set_source_color(gc, &c);
    ps_clear(gc);
}

void WebView::resetPage(void)
{
    m_impl->vposX = 0;
    m_impl->vposY = 0;
    m_impl->voffsetX = 0;
    m_impl->voffsetY = 0;
    m_impl->oldContentX = 0;
    m_impl->oldContentY = 0;
    clear_paint_page();
}

unsigned int WebView::loading(void) const
{
    return m_impl->loading;
}

void WebView::setLoading(unsigned int p, bool finish)
{
    if ((p == 100) && finish) {
        m_impl->loading = 0;

        if (!m_impl->location.empty()) {
            m_impl->newurl = false;
            Application::getInstance()->getHistory()->addHistory(
                Unicode::ConvertUTF16ToUTF8(m_impl->title),
                Unicode::ConvertUTF16ToUTF8(m_impl->location));
        }

        if (m_impl->main->getTabs()->getActiveView() == this) {
            m_impl->main->getAddressBar()->setLoading(false);
            if (!Widget::getFocusedWidget()) {
                setFocus(true);
            }
        }

        autofill_if_needed(); //start auto fill task.

        Application::getInstance()->getMainWindow()->netCheck();

    } else {
        if (m_impl->main->getTabs()->getActiveView() == this) {
            m_impl->main->getAddressBar()->setLoading(true);
        }
        m_impl->loading = ((p == 0) ? 3 : p);
    }
}

bool WebView::isNewUrl(void)
{
    return m_impl->newurl;
}

void WebView::setNewUrl(bool b)
{
    m_impl->newurl = b;
}

void WebView::setFullMode(bool b)
{
    m_impl->fullmode = b;
}

bool WebView::haveMemory(void)
{
    return WebViewPlat::haveMemory();
}

int WebView::virtualOffsetX(void) const
{
    return m_impl->voffsetX;
}

int WebView::virtualOffsetY(void) const
{
    return m_impl->voffsetY;
}

int WebView::getLastPosX(void) const
{
    return m_impl->oldContentX;
}

void WebView::savePosition(int x, int y)
{
    m_impl->oldContentX = x;
    m_impl->oldContentY = y;
}

int WebView::getLastPosY(void) const
{
    return m_impl->oldContentY;
}

void WebView::setViewScroll(int x, int y)
{
    m_impl->scrollX = x;
    m_impl->scrollY = y;

    update_scroll_view_task(m_impl);
}

Rect WebView::getViewportRect(void) const
{
    int px = 0, py = 0;
    macross_view_get_position(m_impl->view, &px, &py);
    px += virtualOffsetX();
    py += virtualOffsetY();
    return Rect(px, py, m_impl->width, m_impl->height);
}

Rect WebView::getContentsRect(void) const
{
    MC_SIZE size = {0, 0};
    int px = 0, py = 0;
    macross_view_get_position(m_impl->view, &px, &py);
    macross_view_get_contents_size(m_impl->view, &size);
    return Rect(px, py, size.w, size.h);
}

void WebView::autofill_if_needed(void)
{
    Widget::postEvent(this, EVENT_FUNC(WebView, autofill_task), EVENT_PARAM(m_impl));
}

void WebView::autofill_task(void* p)
{
    AutoFill::FormData form;
    AutoFill* pa = Application::getInstance()->getAutofill();

    if (m_impl->location.empty()) {
        return;
    }

    std::string url = url_strip_query(Unicode::ConvertUTF16ToUTF8(m_impl->location).c_str());

    if (!pa || !pa->findFormData(url, form)) {
        return;
    }

    std::string script = createFormFillScript(form);

    WebViewImpl* impl = (WebViewImpl*)p;
    macross_view_eval_script(impl->view, script.c_str());
}

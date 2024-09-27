/* mainwindow_qt4.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <QApplication>
#include <picasso/picasso.h>

#include "config.h"
#include "event.h"
#include "mainwindow.h"
#include "mainwindow_qt4.h"
#include "application_qt4.h"

class MainWindowImplPrive
{
public:
    MainWindowImplPrive(MainWindow* main)
        : m_window(main)
        , m_img(0)
        , m_gc(0)
        , m_canvas(0)
        , m_view(0)
        , m_dirty(0, 0, 0, 0)
        , m_vx(0)
        , m_vy(0)
        , m_cx(0)
        , m_cy(0)
        , m_sx(0)
        , m_sy(0)
        , m_hInst(0)
    {
    }

    ~MainWindowImplPrive()
    {
        if (m_gc) {
            ps_context_unref(m_gc);
            m_gc = 0;
        }

        if (m_canvas) {
            ps_canvas_unref(m_canvas);
            m_canvas = 0;
        }

        if (m_img) {
            delete m_img;
            m_img = 0;
        }
    }

    MainWindow* m_window;
    QImage* m_img;
    ps_context* m_gc;
    ps_canvas* m_canvas;
    const QImage* m_view;
    Rect m_dirty;
    int m_vx;
    int m_vy;
    int m_cx;
    int m_cy;
    int m_sx;
    int m_sy;
    void* m_hInst;
};

MainWindowImpl::MainWindowImpl(MainWindow* main)
    : QMainWindow(0, Qt::Dialog)
    , m_data(new MainWindowImplPrive(main))
{
    setMouseTracking(true);
    setAttribute(Qt::WA_InputMethodEnabled);
    setFocusPolicy(Qt::StrongFocus);
}

MainWindowImpl::~MainWindowImpl()
{
    Destroy();
    delete m_data;
}

void* MainWindowImpl::getInstance(void) const
{
    return m_data->m_hInst;
}

bool MainWindowImpl::Create(void* hInstance, const char* title, int x, int y, int w, int h)
{
    m_data->m_hInst = hInstance;

    setWindowTitle(title);
    move(x, y);
    resize(w, h);

    setMinimumSize(width(), height());
    setMaximumSize(width(), height());

    OnCreate(x, y, w, h);

    m_data->m_img = new QImage(w, h, QImage::Format_RGB32);
    m_data->m_img->fill(0xFFFFFFFF);

    m_data->m_canvas = ps_canvas_create_with_data((ps_byte*)m_data->m_img->bits(),
                                                  COLOR_FORMAT_BGRA, w, h, w * 4);
    m_data->m_gc = ps_context_create(m_data->m_canvas, 0);

    show();
    return true;
}

void MainWindowImpl::Destroy(void)
{
    OnDestroy();
    qApp->exit(0);
}

void MainWindowImpl::OnUpdate(int x, int y, int w, int h)
{
    update(x, y, w, h);
}

void MainWindowImpl::OnCreate(int x, int y, int w, int h)
{
    m_data->m_window->OnCreate(0, x, y, w, h);
}

void MainWindowImpl::OnDestroy(void)
{
    m_data->m_window->OnDestroy();
}

void MainWindowImpl::OnMouse(int t, unsigned b, int x, int y)
{
    MouseEvent evt(t, b, x, y);
    m_data->m_window->SendMouseEvent(&evt);
}

void MainWindowImpl::OnKey(int t, unsigned vk)
{
    KeyEvent evt(t, vk);
    m_data->m_window->SendKeyEvent(&evt);
}

void MainWindowImpl::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setClipping(true);
    QRect rect = event->rect();
    OnPaint(&painter, rect.x(), rect.y(), rect.width(), rect.height());
    painter.drawImage(rect, *(m_data->m_img), rect);
}

void MainWindowImpl::OnPaint(QPainter* p, int x, int y, int w, int h)
{
    Rect rc(x, y, w, h);
    m_data->m_window->Paint(m_data->m_gc, &rc);
    paintProcess(p, m_data->m_gc);
}

void MainWindowImpl::OnChar(unsigned int c)
{
    m_data->m_window->CharInput(c);
}

void MainWindowImpl::closeEvent(QCloseEvent* event)
{
    if (m_data->m_window->Destroy()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindowImpl::mousePressEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton) {
        OnMouse(1, 1, event->x(), event->y());
    }
}

void MainWindowImpl::mouseReleaseEvent(QMouseEvent* event)
{
    OnMouse(0, 1, event->x(), event->y());
}

void MainWindowImpl::mouseMoveEvent(QMouseEvent* event)
{
    OnMouse(2, 0, event->x(), event->y());
}

void MainWindowImpl::keyPressEvent(QKeyEvent* event)
{
    OnKey(1, get_virtual_key(event->key()));

    OnChar(*(event->text().utf16()));
}

void MainWindowImpl::keyReleaseEvent(QKeyEvent* event)
{
    OnKey(0, get_virtual_key(event->key()));
}

void MainWindowImpl::inputMethodEvent(QInputMethodEvent* event)
{
    QString str = event->commitString();
    for (int i = 0; i < str.length(); i++) {
        OnChar(str.utf16()[i]);
    }
}

void MainWindowImpl::paintProcess(QPainter* p, ps_context* gc)
{
    if (m_data->m_view && !m_data->m_dirty.isEmpty()) {
        int x = m_data->m_dirty.x + m_data->m_vx;
        int y = m_data->m_dirty.y + m_data->m_vy;
        int w = m_data->m_dirty.w;
        int h = m_data->m_dirty.h;
        QRect cliprect(m_data->m_vx, m_data->m_vy,
                       m_data->m_cx, m_data->m_cy);
        //clip topmost childs area.
        std::list<Widget*> tops = m_data->m_window->getChilds(true);
        p->save();
        if (!tops.empty()) {
            QRegion clip(cliprect);
            QRegion clipouts;
            for (std::list<Widget*>::iterator it = tops.begin(); it != tops.end(); it++) {
                Rect erc(x, y, w, h);
                Rect rc((*it)->boundRect().x, (*it)->boundRect().y, (*it)->boundRect().w, (*it)->boundRect().h);
                if ((*it)->isVisible() && erc.intersect(rc)) {
                    //clip area
                    QRect cxr(erc.x, erc.y, erc.w, erc.h);
                    clipouts = clipouts.united(cxr);
                }
            }
            clip -= clipouts;
            p->setClipRegion(clip);
        }

        QRect dr(x, y, w, h);
        QRect sr(m_data->m_dirty.x - m_data->m_sx, m_data->m_dirty.y - m_data->m_sy, w, h);
        p->drawImage(dr, *(m_data->m_view), sr);
        p->restore();

        if (!tops.empty()) {
            for (std::list<Widget*>::iterator it = tops.begin(); it != tops.end(); it++) {
                Rect erc(x, y, w, h);
                Rect rc((*it)->boundRect().x, (*it)->boundRect().y, (*it)->boundRect().w, (*it)->boundRect().h);
                if ((*it)->isVisible() && erc.intersect(rc)) {
                    //draw top window
                    QRect xerc(erc.x, erc.y, erc.w, erc.h);
                    p->drawImage(xerc, *(m_data->m_img), xerc);
                }
            }
        }

        QRegion tclip(0, 0, m_data->m_window->width(), m_data->m_window->height());
        tclip -= cliprect;
        p->setClipRegion(tclip);
        m_data->m_view = 0;
        m_data->m_dirty = Rect(0, 0, 0, 0);
        m_data->m_vx = 0;
        m_data->m_vy = 0;
    }
}

void MainWindowImpl::DrawImage(const QImage* bmp, int vx, int vy, int cx, int cy, int sx, int sy, int x, int y, int w, int h)
{
    m_data->m_view = bmp;
#if 0
    m_data->m_dirty = Rect(x, y, w, h);
#else
    //FIXME:temp for draw whole area for main page. is it slow
    m_data->m_dirty = Rect(0, 0, cx, cy);
#endif
    m_data->m_vx = vx;
    m_data->m_vy = vy;
    m_data->m_cx = cx;
    m_data->m_cy = cy;
    m_data->m_sx = sx;
    m_data->m_sy = sy;
}

int MainWindowImpl::SysWidth(void)
{
    return DEFAULT_WIDTH;
}

int MainWindowImpl::SysHeight(void)
{
    return DEFAULT_HEIGHT;
}

bool MainWindowImpl::getChooseFile(uchar_t* name, unsigned int len)
{
    return false;
}

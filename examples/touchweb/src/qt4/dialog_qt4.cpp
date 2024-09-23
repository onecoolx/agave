/* dialog_qt4.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include "config.h"
#include "picasso.h"
#include "dialog.h"
#include "dialog_qt4.h"
#include "mainwindow_qt4.h"

class DialogImplPrive
{
public:
	DialogImplPrive(Dialog* d)
		: m_dlg(d), m_img(0), m_gc(0), m_canvas(0)
		, m_x(0), m_y(0), m_w(0), m_h(0)
		, m_inst(0)
	{
	}

	~DialogImplPrive()
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

	Dialog*		m_dlg;
	QImage*		m_img;
	ps_context* m_gc;
	ps_canvas*	m_canvas;
	int         m_x;
	int 		m_y;
	int			m_w;
	int			m_h;
	void*		m_inst;
};

DialogImpl::DialogImpl(Dialog* d)
	: m_data(new DialogImplPrive(d))
{
	setMouseTracking(true);
	setWindowFlags(Qt::Popup);
	setFocusPolicy(Qt::StrongFocus);
	setAttribute(Qt::WA_InputMethodEnabled);
}

DialogImpl::~DialogImpl()
{
	delete m_data;
}

void DialogImpl::OnUpdate(int x, int y, int w, int h)
{
	update(x, y, w, h);
}

void DialogImpl::OnPaint(int x, int y, int w, int h)
{
	Rect rc(x, y, w, h);
	m_data->m_dlg->Paint(m_data->m_gc, &rc);
}

void DialogImpl::OnCreate(int x, int y, int w, int h)
{
	m_data->m_dlg->OnCreate(0, x, y, w, h);
}

void DialogImpl::OnChar(unsigned int c)
{
	m_data->m_dlg->CharInput(c);
}

void DialogImpl::OnMouse(int t, unsigned b, int x, int y)
{
	MouseEvent evt(t, b, x, y);
	m_data->m_dlg->SendMouseEvent(&evt);
}

void DialogImpl::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
	QRect rect = event->rect();
	OnPaint(rect.x(), rect.y(), rect.width(), rect.height());
	painter.drawImage(rect, *(m_data->m_img), rect);
}

void DialogImpl::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton) {
		OnMouse(1, 1, event->x(), event->y());
	}
}

void DialogImpl::mouseReleaseEvent(QMouseEvent *event)
{
	OnMouse(0, 1, event->x(), event->y());
}

void DialogImpl::mouseMoveEvent(QMouseEvent *event)
{
	OnMouse(2, 0, event->x(), event->y());
}

void DialogImpl::keyPressEvent(QKeyEvent *event)
{
	OnChar(*(event->text().utf16()));
}

void DialogImpl::inputMethodEvent(QInputMethodEvent *event)
{
	QString str = event->commitString();
	for(int i = 0; i < str.length(); i++) 
		OnChar(str.utf16()[i]);
}

void DialogImpl::EndModal(int code)
{
	done(Accepted);
	setResult(code);
}

int DialogImpl::DoModal(void)
{
	setModal(true);
	exec();
	return result();
}

void DialogImpl::Create(const MainWindowImpl* main, int x, int y, int w, int h)
{
	m_data->m_inst = main->getInstance();
	m_data->m_x = x;
	m_data->m_y = y;
	m_data->m_w = w;
	m_data->m_h = h;

	QPoint p(x, y);
	p = main->mapToGlobal(p);
	move(p.x(), p.y());
	resize(w, h);

	OnCreate(0, 0, w, h);
	setFocus();
	m_data->m_img = new QImage(w, h, QImage::Format_RGB32);
	m_data->m_img->fill(0xFFFFFFFF);
	m_data->m_canvas = ps_canvas_create_with_data((ps_byte*)m_data->m_img->bits(), 
																COLOR_FORMAT_BGRA, w, h, w*4);
	m_data->m_gc = ps_context_create(m_data->m_canvas);
}


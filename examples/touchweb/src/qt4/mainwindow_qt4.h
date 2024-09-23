/* mainwindow_qt4.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _MAINWINDOW_QT4_H_
#define _MAINWINDOW_QT4_H_

#include <QMainWindow>
#include "picasso.h"

class QImage;
class QMainWindow;
class MainWindowImplPrive;

class MainWindowImpl : public QMainWindow
{
public:
	MainWindowImpl(MainWindow* main);
	virtual ~MainWindowImpl();

	bool Create(void* hInstance, const char* title, int x, int y, int w, int h);
	void Destroy(void);

	void* getInstance(void) const;
	//event handle
	void OnPaint(QPainter* p, int x, int y, int w, int h);
	void OnCreate(int x, int y, int w, int h);
	void OnUpdate(int x, int y, int w, int h);
	void OnChar(unsigned int c);
	void OnDestroy(void);

	void OnMouse(int type, unsigned btn, int x, int y);
	void OnKey(int type, unsigned vk);

	void DrawImage(const QImage* img, int vx, int vy, int cx, int cy, int sx, int sy, int x, int y, int w, int h);
	int SysWidth(void);
	int SysHeight(void);
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
	void closeEvent(QCloseEvent *event);
	void inputMethodEvent(QInputMethodEvent *event);
private:
	void paintProcess(QPainter* p, ps_context* gc);
	MainWindowImplPrive* m_data;
};

#endif/*_MAINWINDOW_QT4_H_*/

/* dialog_qt4.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _DIALOG_QT4_H_
#define _DIALOG_QT4_H_

#include <QDialog>
#include <picasso/picasso.h>

class DialogImplPrive;
class MainWindowImpl;

class DialogImpl : public QDialog
{
public:
    DialogImpl(Dialog* d);
    virtual ~DialogImpl();

    int DoModal(void);
    void EndModal(int code);

    void Create(const MainWindowImpl* main, int x, int y, int w, int h);

    void OnPaint(int x, int y, int w, int h);
    void OnCreate(int x, int y, int w, int h);
    void OnUpdate(int x, int y, int w, int h);
    void OnChar(unsigned int c);
    void OnMouse(int type, unsigned btn, int x, int y);
protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void inputMethodEvent(QInputMethodEvent* event);
private:
    DialogImplPrive* m_data;
};

#endif/*_DIALOG_QT4_H_*/

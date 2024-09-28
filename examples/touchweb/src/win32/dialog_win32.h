/* dialog_win32.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _DIALOG_WIN32_H_
#define _DIALOG_WIN32_H_

#include <windows.h>

class DialogImplPrive;
class MainWindowImpl;

class DialogImpl
{
public:
    DialogImpl(Dialog* d);
    ~DialogImpl();

    int DoModal(void);
    void EndModal(int code);

    void Create(const MainWindowImpl* main, int x, int y, int w, int h);

    void OnPaint(int x, int y, int w, int h);
    void OnCreate(int x, int y, int w, int h);
    void OnUpdate(int x, int y, int w, int h);
    void OnChar(unsigned int c);
    void OnMouse(int type, unsigned btn, int x, int y);

    friend static BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
    DialogImplPrive* m_data;
};

#endif/*_DIALOG_WIN32_H_*/

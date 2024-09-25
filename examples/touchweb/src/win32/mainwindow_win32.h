/* mainwindow_win32.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _MAINWINDOW_WIN32_H_
#define _MAINWINDOW_WIN32_H_

#include <windows.h>
#include "picasso.h"

class MainWindowImplPrive;

class MainWindowImpl
{
public:
    MainWindowImpl(MainWindow* main);
    ~MainWindowImpl();

    bool Create(void* hInstance, const char* title, int x, int y, int w, int h);
    void Destroy(void);

    void* getInstance(void) const;
    HWND getHandle(void) const;
    //event handle
    void OnPaint(int x, int y, int w, int h);
    void OnCreate(int x, int y, int w, int h);
    void OnUpdate(int x, int y, int w, int h);
    void OnChar(unsigned int c);
    void OnDestroy(void);

    void OnMouse(int type, unsigned btn, int x, int y);
    void OnKey(int type, unsigned vk);

    void DrawImage(const BITMAP* bmp, int vx, int vy, int cx, int cy, int sx, int sy, int x, int y, int w, int h);

    int SysWidth(void);
    int SysHeight(void);
    friend static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
    bool getChooseFile(uchar_t* name, unsigned int len);
private:
    void paintProcess(ps_context* gc);
    MainWindowImplPrive* m_data;
};

#endif/*_MAINWINDOW_WIN32_H_*/

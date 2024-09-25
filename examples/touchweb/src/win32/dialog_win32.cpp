/* dialog_win32.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include "picasso.h"
#include "dialog.h"
#include "dialog_win32.h"
#include "mainwindow_win32.h"
#include "application.h"
#include "application_win32.h"

#ifdef WINCE
    #include <aygshell.h>
#endif

class DialogImplPrive
{
public:
    DialogImplPrive(Dialog* d)
        : m_dlg(d), m_bmp(0), m_gc(0), m_canvas(0)
        , m_hWnd(0), m_x(0), m_y(0), m_w(0), m_h(0)
        , m_inst(0), m_hParent(0)
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

        if (m_bmp) {
            free(m_bmp->bmBits);
            delete m_bmp;
            m_bmp = 0;
        }
    }

    Dialog* m_dlg;
    BITMAP* m_bmp;
    ps_context* m_gc;
    ps_canvas* m_canvas;
    HWND m_hWnd;
    int m_x;
    int m_y;
    int m_w;
    int m_h;
    void* m_inst;
    HWND m_hParent;
};

DialogImpl::DialogImpl(Dialog* d)
    : m_data(new DialogImplPrive(d))
{
}

DialogImpl::~DialogImpl()
{
    delete m_data;
}

void DialogImpl::OnPaint(int x, int y, int w, int h)
{
    Rect rc(x, y, w, h);
    m_data->m_dlg->Paint(m_data->m_gc, &rc);
}

void DialogImpl::OnUpdate(int x, int y, int w, int h)
{
    RECT rc = {x, y, x + w, y + h};
    InvalidateRect(m_data->m_hWnd, &rc, FALSE);
}

void DialogImpl::OnMouse(int t, unsigned b, int x, int y)
{
    MouseEvent evt(t, b, x, y);
    m_data->m_dlg->SendMouseEvent(&evt);
}

void DialogImpl::OnChar(unsigned int c)
{
    m_data->m_dlg->CharInput(c);
}

void DialogImpl::OnCreate(int x, int y, int w, int h)
{
    m_data->m_dlg->OnCreate(0, x, y, w, h);
}

void DialogImpl::Create(const MainWindowImpl* main, int x, int y, int w, int h)
{
    m_data->m_inst = main->getInstance();
    m_data->m_x = x;
    m_data->m_y = y;
    m_data->m_w = w;
    m_data->m_h = h;
    m_data->m_hParent = main->getHandle();

    ps_color_format fmt;
    int bit = Application::getInstance()->bits_pixel();
    int byte = Application::getInstance()->bytes_pixel();

    if (byte == 4) {
        fmt = COLOR_FORMAT_BGRA;
    } else if (byte == 2) {
        fmt = COLOR_FORMAT_RGB565;
    }

    m_data->m_bmp = new BITMAP;
    m_data->m_bmp->bmType = 0;
    m_data->m_bmp->bmWidth = w;
    m_data->m_bmp->bmHeight = h;
    m_data->m_bmp->bmWidthBytes = w * byte;
    m_data->m_bmp->bmPlanes = 1;
    m_data->m_bmp->bmBitsPixel = bit;
    m_data->m_bmp->bmBits = malloc(w * h * byte);
    memset(m_data->m_bmp->bmBits, 0xFF, w * h * byte);

    m_data->m_canvas = ps_canvas_create_with_data((ps_byte*)m_data->m_bmp->bmBits, fmt, w, h, w * byte);
    m_data->m_gc = ps_context_create(m_data->m_canvas);
}

struct DLG_BOX {
    DLGTEMPLATE dlgt;
    DWORD ms;
    DWORD cs;
    DWORD ts;
};

static DLG_BOX sdlg = {0};

int DialogImpl::DoModal(void)
{
    ZeroMemory(&sdlg, 0);
    long bu = GetDialogBaseUnits();
    sdlg.dlgt.style = WS_VISIBLE | WS_POPUP;
    sdlg.dlgt.dwExtendedStyle = 0;
    sdlg.dlgt.cdit = 0;
    sdlg.dlgt.x = (m_data->m_x * 4) / LOWORD(bu);
    sdlg.dlgt.y = (m_data->m_y * 8) / HIWORD(bu);
    sdlg.dlgt.cx = (m_data->m_w * 4) / LOWORD(bu);
    sdlg.dlgt.cy = (m_data->m_h * 8) / HIWORD(bu);

    return DialogBoxIndirectParam((HINSTANCE)m_data->m_inst, &sdlg.dlgt, m_data->m_hParent, DlgProc, (LPARAM)this);
}

void DialogImpl::EndModal(int code)
{
    if (m_data->m_hWnd) {
        EndDialog(m_data->m_hWnd, code);
    }
}

static BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    DialogImpl* dlg;

    switch (message) {
        case WM_INITDIALOG: {
                RECT r;
                GetClientRect(hWnd, &r);
                dlg = (DialogImpl*)lParam;
                dlg->m_data->m_hWnd = hWnd;
                SetWindowLong(hWnd, GWL_USERDATA, (LONG)dlg);
                dlg->OnCreate(r.left, r.top, r.right - r.left, r.bottom - r.top);
#ifdef WINCE
                SHFullScreen(hWnd, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON | SHFS_HIDESTARTICON);
#endif
                SetFocus(hWnd);
            }
            return FALSE;
#ifdef WINCE
        case WM_ACTIVATE: {
                SHFullScreen(hWnd, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON | SHFS_HIDESTARTICON);
            }
            return TRUE;
#endif
        case WM_LBUTTONDOWN: {
                unsigned b = 0;
                if (wParam & MK_LBUTTON) {
                    b |= 1;
                }
                if (wParam & MK_RBUTTON) {
                    b |= 2;
                }
                dlg = (DialogImpl*)GetWindowLong(hWnd, GWL_USERDATA);
                dlg->OnMouse(1, b, LOWORD(lParam), HIWORD(lParam));
            }
            return TRUE;
        case WM_LBUTTONUP: {
                unsigned b = 0;
                if (wParam & MK_LBUTTON) {
                    b |= 1;
                }
                if (wParam & MK_RBUTTON) {
                    b |= 2;
                }
                dlg = (DialogImpl*)GetWindowLong(hWnd, GWL_USERDATA);
                dlg->OnMouse(0, b, LOWORD(lParam), HIWORD(lParam));
            }
            return TRUE;
        case WM_MOUSEMOVE: {
                unsigned b = 0;
                if (wParam & MK_LBUTTON) {
                    b |= 1;
                }
                if (wParam & MK_RBUTTON) {
                    b |= 2;
                }
                dlg = (DialogImpl*)GetWindowLong(hWnd, GWL_USERDATA);
                dlg->OnMouse(2, b, LOWORD(lParam), HIWORD(lParam));
            }
            return TRUE;
        case WM_IME_CHAR: {
                wchar_t uc = 0;
                unsigned char mc[3] = {0};
                mc[0] = (wParam >> 8) & 0xFF;
                mc[1] = wParam & 0xFF;
                ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (char*)&mc, -1, &uc, 1);
                wParam = uc;
            }
        case WM_CHAR: {
                dlg = (DialogImpl*)GetWindowLong(hWnd, GWL_USERDATA);
                dlg->OnChar(wParam);
            }
            return TRUE;
        case WM_KEYDOWN: {
                MSG msg;
                if (GetMessage(&msg, hWnd, WM_KEYFIRST, WM_KEYLAST) > 0) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
            return TRUE;
        case WM_SETFOCUS:
            return TRUE;
        case WM_ERASEBKGND:
            return TRUE;
        case WM_PAINT: {
                HDC mdc;
                HGDIOBJ h;
                HBITMAP hbmp;
                BITMAP* pm;
                RECT crc;
                dlg = (DialogImpl*)GetWindowLong(hWnd, GWL_USERDATA);
                if (GetUpdateRect(hWnd, &crc, FALSE)) {
                    dlg->OnPaint(crc.left, crc.top, crc.right - crc.left, crc.bottom - crc.top);
                    hdc = BeginPaint(hWnd, &ps);
                    mdc = CreateCompatibleDC(hdc);
                    pm = dlg->m_data->m_bmp;
                    hbmp = CreateBitmap(pm->bmWidth, pm->bmHeight, pm->bmPlanes, pm->bmBitsPixel, pm->bmBits);
                    h = SelectObject(mdc, hbmp);
                    BitBlt(hdc, crc.left, crc.top, crc.right - crc.left, crc.bottom - crc.top,
                           mdc, crc.left, crc.top, SRCCOPY);
                    SelectObject(mdc, h);
                    DeleteObject(hbmp);
                    DeleteDC(mdc);
                    EndPaint(hWnd, &ps);
                }
            }
            return TRUE;
        default:
            return FALSE;
    }
}

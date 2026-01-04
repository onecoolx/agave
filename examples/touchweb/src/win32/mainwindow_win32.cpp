/* mainwindow_win32.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include <stdio.h>
#include "config.h"
#include <picasso/picasso.h>

#include "event.h"
#include "mainwindow.h"
#include "mainwindow_win32.h"
#include "application.h"
#include "application_win32.h"

HWND g_MainWnd = 0;

static ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = (WNDPROC)WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = 0;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = L"Agave";
    wcex.hIconSm = 0;

    return RegisterClassEx(&wcex);
}

class MainWindowImplPrive
{
public:
    MainWindowImplPrive(MainWindow* main)
        : m_window(main)
        , m_bmp(0)
        , m_back(0)
        , m_gc(0)
        , m_canvas(0)
        , m_hWnd(0)
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

        if (m_bmp) {
            free(m_bmp->bmBits);
            delete m_bmp;
            m_bmp = 0;
        }

        if (m_back) {
            DeleteObject(m_back);
            m_back = 0;
        }
    }

    MainWindow* m_window;
    BITMAP* m_bmp;
    HBITMAP m_back;
    ps_context* m_gc;
    ps_canvas* m_canvas;
    HWND m_hWnd;
    const BITMAP* m_view;
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
    : m_data(new MainWindowImplPrive(main))
{
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

HWND MainWindowImpl::getHandle(void) const
{
    return m_data->m_hWnd;
}

bool MainWindowImpl::Create(void* hInstance, const char* title, int x, int y, int w, int h)
{
    static bool initialize = false;
    if (!initialize) {
        MyRegisterClass((HINSTANCE)hInstance);
        initialize = true;
    }

    if (!ps_initialize()) {
        return false;
    }

    m_data->m_hInst = hInstance;
    g_MainWnd = m_data->m_hWnd = CreateWindowExA(WS_EX_APPWINDOW, "Agave", title, WS_OVERLAPPEDWINDOW, x, y, w, h + GetSystemMetrics(SM_CYCAPTION),
                                                 NULL, NULL, (HINSTANCE)hInstance, this);

    if (!m_data->m_hWnd) {
        return false;
    }

    ps_color_format fmt;
    int bit = Application::getInstance()->bits_pixel();
    int byte = Application::getInstance()->bytes_pixel();

    if (byte == 4) {
        fmt = COLOR_FORMAT_BGRA;
    } else if (byte == 2) {
        fmt = COLOR_FORMAT_RGB565;
    } else {
        return false;
    }

    m_data->m_bmp = new BITMAP;
    m_data->m_bmp->bmType = 0;
    m_data->m_bmp->bmWidth = w;
    m_data->m_bmp->bmHeight = h;
    m_data->m_bmp->bmWidthBytes = w * byte;
    m_data->m_bmp->bmPlanes = 1;
    m_data->m_bmp->bmBitsPixel = bit;
    m_data->m_bmp->bmBits = malloc(w * h * byte);
    memset(m_data->m_bmp->bmBits, (char)0xFF, w * h * byte);

    HDC hdc = GetDC(m_data->m_hWnd);
    m_data->m_back = CreateCompatibleBitmap(hdc, w, h);
    ReleaseDC(m_data->m_hWnd, hdc);

    m_data->m_canvas = ps_canvas_create_with_data((ps_byte*)m_data->m_bmp->bmBits, fmt, w, h, w * byte);
    m_data->m_gc = ps_context_create(m_data->m_canvas, 0);

    ShowWindow(m_data->m_hWnd, SW_SHOW);
    UpdateWindow(m_data->m_hWnd);

    return true;
}

void MainWindowImpl::Destroy(void)
{
    if (m_data->m_hWnd) {
        DestroyWindow(m_data->m_hWnd);
        m_data->m_hWnd = 0;
    }
}

void MainWindowImpl::OnUpdate(int x, int y, int w, int h)
{
    RECT rc = {x, y, x + w, y + h};
    InvalidateRect(m_data->m_hWnd, &rc, TRUE);
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

void MainWindowImpl::OnPaint(int x, int y, int w, int h)
{
    Rect rc(x, y, w, h);
    m_data->m_window->Paint(m_data->m_gc, &rc);
    paintProcess(m_data->m_gc);
}

void MainWindowImpl::OnChar(unsigned int c)
{
    m_data->m_window->CharInput(c);
}

void MainWindowImpl::paintProcess(ps_context* gc)
{
    if (m_data->m_view && !m_data->m_dirty.isEmpty()) {
        int x = m_data->m_dirty.x + m_data->m_vx;
        int y = m_data->m_dirty.y + m_data->m_vy;
        int w = m_data->m_dirty.w;
        int h = m_data->m_dirty.h;
        HDC hdc = GetDC(m_data->m_hWnd);
        HDC mdc = CreateCompatibleDC(hdc);
        HDC bdc = CreateCompatibleDC(hdc);
        //clip topmost childs area.
        std::list<Widget*> tops = m_data->m_window->getChilds(true);
        SaveDC(hdc);
        if (!tops.empty()) {
            for (std::list<Widget*>::iterator it = tops.begin(); it != tops.end(); it++) {
                Rect erc(x, y, w, h);
                Rect rc((*it)->boundRect().x, (*it)->boundRect().y, (*it)->boundRect().w, (*it)->boundRect().h);
                if ((*it)->isVisible() && erc.intersect(rc)) {
                    //clip area
                    ExcludeClipRect(hdc, erc.x, erc.y, erc.x + erc.w, erc.y + erc.h);
                }
            }
        }
        const BITMAP* pm = m_data->m_view;
        HBITMAP hbmp = CreateBitmap(pm->bmWidth, pm->bmHeight, pm->bmPlanes, pm->bmBitsPixel, pm->bmBits);

        HGDIOBJ hc = SelectObject(bdc, m_data->m_back);
        //clear back storage dc
        RECT frc = {m_data->m_vx, m_data->m_vy, m_data->m_vx + m_data->m_cx, m_data->m_vy + m_data->m_cy};
        FillRect(bdc, &frc, (HBRUSH)GetStockObject(WHITE_BRUSH));

        //draw to back storage dc
        HGDIOBJ hd = SelectObject(mdc, hbmp);
        BitBlt(bdc, x, y, w, h, mdc, m_data->m_dirty.x - m_data->m_sx, m_data->m_dirty.y - m_data->m_sy, SRCCOPY);
        SelectObject(mdc, hd);
        //paint to window
        BitBlt(hdc, x, y, w, h, bdc, x, y, SRCCOPY);

        SelectObject(mdc, hc);
        DeleteObject(hbmp);
        RestoreDC(hdc, -1);

        RECT clip = {m_data->m_vx, m_data->m_vy,
                     m_data->m_vx + m_data->m_cx,
                     m_data->m_vy + m_data->m_cy
                    };

        if (!tops.empty()) {
            pm = m_data->m_bmp;
            hbmp = CreateBitmap(pm->bmWidth, pm->bmHeight, pm->bmPlanes, pm->bmBitsPixel, pm->bmBits);
            hd = SelectObject(mdc, hbmp);

            for (std::list<Widget*>::iterator it = tops.begin(); it != tops.end(); it++) {
                Rect erc(x, y, w, h);
                Rect rc((*it)->boundRect().x, (*it)->boundRect().y, (*it)->boundRect().w, (*it)->boundRect().h);
                if ((*it)->isVisible() && erc.intersect(rc)) {
                    //draw top window
                    BitBlt(hdc, erc.x, erc.y, erc.w, erc.h, mdc, erc.x, erc.y, SRCCOPY);
                }
            }

            SelectObject(mdc, hd);
            DeleteObject(hbmp);
        }

        ValidateRect(m_data->m_hWnd, &clip); //do not needed paint image area yet.
        DeleteDC(bdc);
        DeleteDC(mdc);
        ReleaseDC(m_data->m_hWnd, hdc);
        m_data->m_view = 0;
        m_data->m_dirty = Rect(0, 0, 0, 0);
        m_data->m_vx = 0;
        m_data->m_vy = 0;
    }
}

void MainWindowImpl::DrawImage(const BITMAP* bmp, int vx, int vy, int cx, int cy, int sx, int sy, int x, int y, int w, int h)
{
    m_data->m_view = bmp;
    m_data->m_dirty = Rect(0, 0, cx, cy);
    m_data->m_vx = vx;
    m_data->m_vy = vy;
    m_data->m_cx = cx;
    m_data->m_cy = cy;
    m_data->m_sx = sx;
    m_data->m_sy = sy;
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    MainWindowImpl* window;

    switch (message) {
        case WM_CREATE: {
                RECT r;
                GetClientRect(hWnd, &r);
                LPCREATESTRUCT cs = (LPCREATESTRUCT)lParam;
                SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
                window = (MainWindowImpl*)cs->lpCreateParams;
                window->OnCreate(r.left, r.top, r.right - r.left, r.bottom - r.top);
            }
            break;
        case WM_LBUTTONDOWN: {
                window = (MainWindowImpl*)GetWindowLongPtr(hWnd, GWL_USERDATA);
                window->OnMouse(1, 1, LOWORD(lParam), HIWORD(lParam));
            }
            break;
        case WM_LBUTTONUP: {
                window = (MainWindowImpl*)GetWindowLongPtr(hWnd, GWL_USERDATA);
                window->OnMouse(0, 1, LOWORD(lParam), HIWORD(lParam));
            }
            break;
        case WM_MOUSEMOVE: {
                window = (MainWindowImpl*)GetWindowLongPtr(hWnd, GWL_USERDATA);
                window->OnMouse(2, 0, LOWORD(lParam), HIWORD(lParam));
            }
            break;
        case WM_KEYDOWN: {
                window = (MainWindowImpl*)GetWindowLongPtr(hWnd, GWL_USERDATA);
                window->OnKey(1, wParam);
            }
            break;
        case WM_KEYUP: {
                window = (MainWindowImpl*)GetWindowLongPtr(hWnd, GWL_USERDATA);
                window->OnKey(0, wParam);
            }
            break;
        case WM_IME_CHAR: {
                wchar_t uc = 0;
                unsigned char mc[3] = {0};
                mc[0] = (wParam >> 8) & 0xFF;
                mc[1] = wParam & 0xFF;
                ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (char*)&mc, -1, &uc, 1);
                wParam = uc;
            }
        case WM_CHAR: {
                window = (MainWindowImpl*)GetWindowLongPtr(hWnd, GWL_USERDATA);
                window->OnChar(wParam);
            }
            break;
        case WM_ERASEBKGND:
            break;
        case WM_PAINT: {
                HDC mdc;
                HGDIOBJ h;
                HBITMAP hbmp;
                BITMAP* pm;
                RECT crc;
                window = (MainWindowImpl*)GetWindowLongPtr(hWnd, GWL_USERDATA);
                if (GetUpdateRect(hWnd, &crc, FALSE)) {
                    window->OnPaint(crc.left, crc.top, crc.right - crc.left, crc.bottom - crc.top);
                    hdc = BeginPaint(hWnd, &ps);
                    mdc = CreateCompatibleDC(hdc);
                    pm = window->m_data->m_bmp;
                    hbmp = CreateBitmap(pm->bmWidth, pm->bmHeight, pm->bmPlanes, pm->bmBitsPixel, pm->bmBits);
                    h = SelectObject(mdc, hbmp);
                    BitBlt(hdc, crc.left, crc.top, crc.right - crc.left, crc.bottom - crc.top, mdc, crc.left, crc.top, SRCCOPY);
                    SelectObject(mdc, h);
                    DeleteObject(hbmp);
                    DeleteDC(mdc);
                    EndPaint(hWnd, &ps);
                }
            }
            break;
        case WM_CLOSE: {
                window = (MainWindowImpl*)GetWindowLongPtr(hWnd, GWL_USERDATA);
                window->m_data->m_window->Destroy();
            }
            break;
        case WM_DESTROY: {
                window = (MainWindowImpl*)GetWindowLongPtr(hWnd, GWL_USERDATA);
                PostQuitMessage(0);
                window->OnDestroy();
            }
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

bool MainWindowImpl::getChooseFile(uchar_t* name, unsigned int len)
{
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = g_MainWnd;

    ofn.lpstrFile = name;
    ofn.nMaxFile = len;
    ofn.lpstrFilter = L"All\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        return true;
    } else {
        return false;
    }
    return false;
}

int MainWindowImpl::SysWidth(void)
{
    return DEFAULT_WIDTH;
}

int MainWindowImpl::SysHeight(void)
{
    return DEFAULT_HEIGHT;
}

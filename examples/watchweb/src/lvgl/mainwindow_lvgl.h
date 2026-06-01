/* mainwindow_lvgl.h - WatchWeb LVGL platform backend
 *
 * Copyright (C) 2026 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _MAINWINDOW_LVGL_H_
#define _MAINWINDOW_LVGL_H_

#include <picasso/picasso.h>
#include "lvgl.h"

class MainWindow;

class MainWindowImpl {
public:
    MainWindowImpl(MainWindow* main);
    ~MainWindowImpl();

    bool Create(void* hInstance, const char* title, int x, int y, int w, int h);
    void Destroy(void);

    void* getInstance(void) const { return m_hInst; }

    void OnPaint(int x, int y, int w, int h);
    void OnCreate(int x, int y, int w, int h);
    void OnUpdate(int x, int y, int w, int h);
    void OnDestroy(void);
    void OnMouse(int type, unsigned btn, int x, int y);
    void OnKey(int type, unsigned vk);
    void OnChar(unsigned int c);

    void DrawImage(const void* img, int vx, int vy, int cx, int cy,
                   int sx, int sy, int x, int y, int w, int h);
    int SysWidth(void) { return DEFAULT_WIDTH; }
    int SysHeight(void) { return DEFAULT_HEIGHT; }

    bool getChooseFile(uchar_t* name, unsigned int len) { return false; }

    /* LVGL event processing */
    void processEvents(void);

private:
    static void sdl_event_cb(lv_event_t* e);

    MainWindow* m_window;
    ps_context* m_gc;
    ps_canvas* m_canvas;
    unsigned char* m_buffer;
    void* m_hInst;
    lv_obj_t* m_img_obj;  /* LVGL image object for display */
    lv_image_dsc_t m_img_dsc;
};

#endif /* _MAINWINDOW_LVGL_H_ */

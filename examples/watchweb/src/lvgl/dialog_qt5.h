/* dialog_qt5.h - WatchWeb LVGL stub */
#ifndef _DIALOG_LVGL_H_
#define _DIALOG_LVGL_H_

#include <picasso/picasso.h>

class Dialog;
class MainWindowImpl;

class DialogImpl {
public:
    DialogImpl(Dialog* d) : m_dialog(d) {}
    ~DialogImpl() {}

    int DoModal(void) { return 0; }
    void EndModal(int code) { (void)code; }
    void Create(const MainWindowImpl* main, int x, int y, int w, int h)
    { (void)main; (void)x; (void)y; (void)w; (void)h; }

    void OnPaint(int x, int y, int w, int h) { (void)x; (void)y; (void)w; (void)h; }
    void OnCreate(int x, int y, int w, int h) { (void)x; (void)y; (void)w; (void)h; }
    void OnUpdate(int x, int y, int w, int h) { (void)x; (void)y; (void)w; (void)h; }
    void OnChar(unsigned int c) { (void)c; }
    void OnMouse(int type, unsigned btn, int x, int y) { (void)type; (void)btn; (void)x; (void)y; }

private:
    Dialog* m_dialog;
};

#endif /* _DIALOG_LVGL_H_ */

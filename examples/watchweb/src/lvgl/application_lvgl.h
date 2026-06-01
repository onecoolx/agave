/* application_lvgl.h - WatchWeb LVGL platform backend
 *
 * Copyright (C) 2026 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _APPLICATION_LVGL_H_
#define _APPLICATION_LVGL_H_

#include <picasso/picasso.h>

class Application;

class ApplicationImpl {
public:
    ApplicationImpl(Application*);
    ~ApplicationImpl();

    void init(void);
    int run_loop(void);

    ps_color_format color_format(void) const { return COLOR_FORMAT_BGRA; }
    int bits_pixel(void) const { return 32; }
    int bytes_pixel(void) const { return 4; }
    int today(void) const;
    bool macAddress(std::string&) { return false; }

    static unsigned long tickCount(void);
private:
    Application* m_app;
    bool m_running;
};

void SetImeStatus_platform(bool b);
bool ImeIsShow_platform(void);
void System_init(void);
int get_virtual_key(int pk);

#endif /* _APPLICATION_LVGL_H_ */

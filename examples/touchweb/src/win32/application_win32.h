/* application_win32.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _APPLICATION_WIN32_H_
#define _APPLICATION_WIN32_H_

class Application;
class ApplicationImpl
{
public:
    ApplicationImpl(Application*);
    ~ApplicationImpl();

    int screen_width(void) const;
    int screen_height(void) const;

    void init(void);
    int run_loop(void);

    ps_color_format color_format(void) const { return m_format; }
    int bits_pixel(void) const { return m_cbit; }
    int bytes_pixel(void) const { return m_cbyte; }
    int today(void) const;
    bool macAddress(std::string&);

    static unsigned long tickCount(void);
public:
    Application* getApp(void) const { return m_data; }
private:
    Application* m_data;
    int m_cbit;
    int m_cbyte;
    ps_color_format m_format;
};

void SetImeStatus_platform(bool b);
bool ImeIsShow_platform(void);
void System_init(void);
#endif /*_APPLICATION_WIN32_H_*/

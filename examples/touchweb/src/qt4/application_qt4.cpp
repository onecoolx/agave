/* application_qt4.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <map>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/if_ether.h>

#include "macross.h"
#include "application.h"
#include "application_qt4.h"

static void init_key_map(void);

ApplicationImpl::ApplicationImpl(Application* app)
    : QApplication(0, 0)
    , m_data(app)
    , m_cbit(0)
    , m_cbyte(0)
    , m_format(COLOR_FORMAT_UNKNOWN)
{
    m_cbit = 32;
    m_cbyte = m_cbit / 8;
    m_format = COLOR_FORMAT_BGRA;
    init_key_map();
    macross_initialize(PIXEL_FORMAT_BGRA32, screen_width(), screen_height());
    start(10);
}

ApplicationImpl::~ApplicationImpl()
{
    macross_shutdown();
}

void ApplicationImpl::timerEvent(QTimerEvent* timer)
{
    if (!QCoreApplication::hasPendingEvents()) {
        if (!m_data->event_loop()) {
            m_data->idle_loop();
        }
        m_data->loop_callback();
    } else {
        m_data->event_loop();
    }

}

unsigned long ApplicationImpl::tickCount(void)
{
    return clock();
}

int ApplicationImpl::today(void) const
{
    int today = 0;
    time_t tm = 0;
    struct tm* ptm = 0;

    tm = time(0);
    ptm = localtime(&tm);

    today = ptm->tm_year + 1900;
    today *= 100;
    today += ptm->tm_mon + 1;
    today *= 100;
    today += ptm->tm_mday;
    return today;
}

bool ApplicationImpl::macAddress(std::string& retstr)
{
    int socketfd = 0;
    struct ifreq req;

    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return false;
    }

    memset(&req, 0, sizeof(struct ifreq));
    strncpy(req.ifr_name, "eth0", sizeof(req.ifr_name));

    if (ioctl(socketfd, SIOCGIFHWADDR, &req) < 0) {
        return false;
    }

    char inmac[16] = {0};
    sprintf(inmac, "%02X%02X%02X%02X%02X%02X",
            (unsigned char)req.ifr_hwaddr.sa_data[0],
            (unsigned char)req.ifr_hwaddr.sa_data[1],
            (unsigned char)req.ifr_hwaddr.sa_data[2],
            (unsigned char)req.ifr_hwaddr.sa_data[3],
            (unsigned char)req.ifr_hwaddr.sa_data[4],
            (unsigned char)req.ifr_hwaddr.sa_data[5]);

    retstr = std::string(inmac);
    return true;
}

int ApplicationImpl::screen_height(void) const
{
    return QApplication::desktop()->screenGeometry().height();
}

int ApplicationImpl::screen_width(void) const
{
    return QApplication::desktop()->screenGeometry().width();
}

int ApplicationImpl::run_loop(void)
{
    return exec();
}

typedef struct {
    int vk;
    int pk;
} KeyEntities;

static KeyEntities key_map[] = {
    {KEY_BACK, Qt::Key_Backspace},
    {KEY_TAB, Qt::Key_Tab},
    {KEY_CLEAR, Qt::Key_Clear},
    {KEY_ENTER, Qt::Key_Return},
    {KEY_SHIFT, Qt::Key_Shift},
    {KEY_CTRL, Qt::Key_Control},
    {KEY_ALT, Qt::Key_Menu},
    {KEY_PAUSE, Qt::Key_Pause},
    {KEY_CAPSLOCK, Qt::Key_CapsLock},
    {KEY_ESCAPE, Qt::Key_Escape},
    {KEY_SPACE, Qt::Key_Space},
    {KEY_PAGEUP, Qt::Key_PageUp},
    {KEY_PAGEDOWN, Qt::Key_PageDown},
    {KEY_END, Qt::Key_End},
    {KEY_HOME, Qt::Key_Home},
    {KEY_LEFT, Qt::Key_Left},
    {KEY_UP, Qt::Key_Up},
    {KEY_RIGHT, Qt::Key_Right},
    {KEY_DOWN, Qt::Key_Down},
    {KEY_SELECT, Qt::Key_Select},
    {KEY_PRINT, Qt::Key_Print},
    {KEY_EXEC, Qt::Key_Execute},
    {KEY_INSERT, Qt::Key_Insert},
    {KEY_DELETE, Qt::Key_Delete},
    {KEY_HELP, Qt::Key_Help},
    {KEY_0, Qt::Key_0},
    {KEY_1, Qt::Key_1},
    {KEY_2, Qt::Key_2},
    {KEY_3, Qt::Key_3},
    {KEY_4, Qt::Key_4},
    {KEY_5, Qt::Key_5},
    {KEY_6, Qt::Key_6},
    {KEY_7, Qt::Key_7},
    {KEY_8, Qt::Key_8},
    {KEY_9, Qt::Key_9},
    {KEY_A, Qt::Key_A},
    {KEY_B, Qt::Key_B},
    {KEY_C, Qt::Key_C},
    {KEY_D, Qt::Key_D},
    {KEY_E, Qt::Key_E},
    {KEY_F, Qt::Key_F},
    {KEY_G, Qt::Key_G},
    {KEY_H, Qt::Key_H},
    {KEY_I, Qt::Key_I},
    {KEY_J, Qt::Key_J},
    {KEY_K, Qt::Key_K},
    {KEY_L, Qt::Key_L},
    {KEY_M, Qt::Key_M},
    {KEY_N, Qt::Key_N},
    {KEY_O, Qt::Key_O},
    {KEY_P, Qt::Key_P},
    {KEY_Q, Qt::Key_Q},
    {KEY_R, Qt::Key_R},
    {KEY_S, Qt::Key_S},
    {KEY_T, Qt::Key_T},
    {KEY_U, Qt::Key_U},
    {KEY_V, Qt::Key_V},
    {KEY_W, Qt::Key_W},
    {KEY_X, Qt::Key_X},
    {KEY_Y, Qt::Key_Y},
    {KEY_Z, Qt::Key_Z},
    {KEY_LWIN, Qt::Key_Meta},
    {KEY_RWIN, Qt::Key_Meta},
    {KEY_EQUAL, Qt::Key_Equal},
    {KEY_MINUS, Qt::Key_Minus},
    {KEY_DECIMAL, Qt::Key_Greater},
    {KEY_SLASH, Qt::Key_Question},
    {KEY_F1, Qt::Key_F1},
    {KEY_F2, Qt::Key_F2},
    {KEY_F3, Qt::Key_F3},
    {KEY_F4, Qt::Key_F4},
    {KEY_F5, Qt::Key_F5},
    {KEY_F6, Qt::Key_F6},
    {KEY_F7, Qt::Key_F7},
    {KEY_F8, Qt::Key_F8},
    {KEY_F9, Qt::Key_F9},
    {KEY_F10, Qt::Key_F10},
    {KEY_F11, Qt::Key_F11},
    {KEY_F12, Qt::Key_F12},
    {KEY_F13, Qt::Key_F13},
    {KEY_F14, Qt::Key_F14},
    {KEY_F15, Qt::Key_F15},
    {KEY_F16, Qt::Key_F16},
    {KEY_F17, Qt::Key_F17},
    {KEY_F18, Qt::Key_F18},
    {KEY_F19, Qt::Key_F19},
    {KEY_F20, Qt::Key_F20},
    {KEY_F21, Qt::Key_F21},
    {KEY_F22, Qt::Key_F22},
    {KEY_F23, Qt::Key_F23},
    {KEY_F24, Qt::Key_F24},
};

static std::map<int, int> g_key_map;

static void init_key_map(void)
{
    for (int i = 0; i < (sizeof(key_map) / sizeof(KeyEntities)); i++) {
        g_key_map[key_map[i].pk] = key_map[i].vk;
    }
}

int get_virtual_key(int pk)
{
    return g_key_map[pk];
}

void SetImeStatus_platform(bool b)
{
}

bool ImeIsShow_platform(void)
{
    return false;
}

void System_init(void)
{
}

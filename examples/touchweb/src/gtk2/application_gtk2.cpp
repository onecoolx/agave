/* application_gtk2.cpp - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
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
#include "application_gtk2.h"

static void init_key_map(void);

ApplicationImpl::ApplicationImpl(Application* app)
    : m_data(app)
    , m_cbit(0)
    , m_cbyte(0)
    , m_format(COLOR_FORMAT_UNKNOWN)
{
    m_cbit = 24;
    m_cbyte = m_cbit / 8;
    m_format = COLOR_FORMAT_RGB;
    init_key_map();
    macross_initialize(PIXEL_FORMAT_RGB24, screen_width(), screen_height());
    start(10);
}

ApplicationImpl::~ApplicationImpl()
{
    macross_shutdown();
}

void ApplicationImpl::idleEvent(void)
{
    if (!m_data->event_loop()) {
        m_data->idle_loop();
    }
    m_data->loop_callback();
}

void ApplicationImpl::timerEvent(void)
{
    m_data->event_loop();
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
    return gdk_screen_height();
}

int ApplicationImpl::screen_width(void) const
{
    return gdk_screen_width();
}

void ApplicationImpl::init(void)
{
    gtk_init(NULL, NULL);
}

int ApplicationImpl::run_loop(void)
{
    gtk_main();
    return 0;
}

typedef struct {
    int vk;
    int pk;
} KeyEntities;

static KeyEntities key_map[] = {
    {KEY_BACK, GDK_BackSpace},
    {KEY_TAB, GDK_Tab},
    {KEY_CLEAR, GDK_Clear},
    {KEY_ENTER, GDK_Return},
    {KEY_SHIFT, GDK_Shift_L},
    {KEY_CTRL, GDK_Control_L},
    {KEY_ALT, GDK_Menu},
    {KEY_PAUSE, GDK_Pause},
    {KEY_CAPSLOCK, GDK_Caps_Lock},
    {KEY_ESCAPE, GDK_Escape},
    {KEY_SPACE, GDK_space},
    {KEY_PAGEUP, GDK_Prior},
    {KEY_PAGEDOWN, GDK_Next},
    {KEY_END, GDK_End},
    {KEY_HOME, GDK_Home},
    {KEY_LEFT, GDK_Left},
    {KEY_UP, GDK_Up},
    {KEY_RIGHT, GDK_Right},
    {KEY_DOWN, GDK_Down},
    {KEY_SELECT, GDK_Select},
    {KEY_PRINT, GDK_Print},
    {KEY_EXEC, GDK_Execute},
    {KEY_INSERT, GDK_Insert},
    {KEY_DELETE, GDK_Delete},
    {KEY_HELP, GDK_Help},
    {KEY_0, GDK_0},
    {KEY_1, GDK_1},
    {KEY_2, GDK_2},
    {KEY_3, GDK_3},
    {KEY_4, GDK_4},
    {KEY_5, GDK_5},
    {KEY_6, GDK_6},
    {KEY_7, GDK_7},
    {KEY_8, GDK_8},
    {KEY_9, GDK_9},
    {KEY_A, GDK_a},
    {KEY_B, GDK_b},
    {KEY_C, GDK_c},
    {KEY_D, GDK_d},
    {KEY_E, GDK_e},
    {KEY_F, GDK_f},
    {KEY_G, GDK_g},
    {KEY_H, GDK_h},
    {KEY_I, GDK_i},
    {KEY_J, GDK_j},
    {KEY_K, GDK_k},
    {KEY_L, GDK_l},
    {KEY_M, GDK_m},
    {KEY_N, GDK_n},
    {KEY_O, GDK_o},
    {KEY_P, GDK_p},
    {KEY_Q, GDK_q},
    {KEY_R, GDK_r},
    {KEY_S, GDK_s},
    {KEY_T, GDK_t},
    {KEY_U, GDK_u},
    {KEY_V, GDK_v},
    {KEY_W, GDK_w},
    {KEY_X, GDK_x},
    {KEY_Y, GDK_y},
    {KEY_Z, GDK_z},
    {KEY_LWIN, GDK_Meta_L},
    {KEY_RWIN, GDK_Meta_R},
    {KEY_EQUAL, GDK_equal},
    {KEY_MINUS, GDK_minus},
    {KEY_DECIMAL, GDK_decimalpoint},
    {KEY_DIVIDE, GDK_division},
    {KEY_F1, GDK_F1},
    {KEY_F2, GDK_F2},
    {KEY_F3, GDK_F3},
    {KEY_F4, GDK_F4},
    {KEY_F5, GDK_F5},
    {KEY_F6, GDK_F6},
    {KEY_F7, GDK_F7},
    {KEY_F8, GDK_F8},
    {KEY_F9, GDK_F9},
    {KEY_F10, GDK_F10},
    {KEY_F11, GDK_F11},
    {KEY_F12, GDK_F12},
    {KEY_F13, GDK_F13},
    {KEY_F14, GDK_F14},
    {KEY_F15, GDK_F15},
    {KEY_F16, GDK_F16},
    {KEY_F17, GDK_F17},
    {KEY_F18, GDK_F18},
    {KEY_F19, GDK_F19},
    {KEY_F20, GDK_F20},
    {KEY_F21, GDK_F21},
    {KEY_F22, GDK_F22},
    {KEY_F23, GDK_F23},
    {KEY_F24, GDK_F24},
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

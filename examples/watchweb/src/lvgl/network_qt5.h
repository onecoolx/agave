/* network_qt5.h - WatchWeb LVGL stub */
#ifndef _NETWORK_LVGL_H_
#define _NETWORK_LVGL_H_

class NetService;

class NetServiceImpl {
public:
    NetServiceImpl(NetService*) {}
    ~NetServiceImpl() {}
    bool isAvaliable(void) { return true; }
    bool establishConnection(void) { return true; }
    bool establishConnectionSync(void) { return true; }
    bool isConnected(void) { return true; }
    void readNetConfigs() {}
};

#endif

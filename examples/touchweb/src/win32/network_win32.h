/* network_win32.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _NETWORK_WIN32_H_
#define _NETWORK_WIN32_H_

#include <vector>

class NetService;

enum ConnType {
    ConnType_Direct,
    ConnType_Proxy,
};

struct NetConfItemImpl {
#if defined(WINCE)
    int index;
    GUID guid; //DestNetwork
#endif
};

class NetServiceImpl
{
public:
    NetServiceImpl(NetService* service);
    ~NetServiceImpl();

    bool isAvaliable(void);
    bool establishConnection(ConnType t = ConnType_Direct);
    bool establishConnectionSync(ConnType t = ConnType_Direct);
    bool isConnected(void);

    void readNetConfigs();
private:
    void checkConnectType(void);
    void releaseConnection(void);
    NetService* m_service;
#if defined(WINCE)
    bool getConnectedGUID(GUID* guid);
    HANDLE m_hConn;
    DWORD m_index;
    GUID m_guid;
#endif
};

#endif /*_NETWORK_WIN32_H_*/

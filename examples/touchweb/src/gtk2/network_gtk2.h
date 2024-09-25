/* network_gtk2.h - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _NETWORK_GTK2_H_
#define _NETWORK_GTK2_H_

class NetService;

class NetServiceImpl
{
public:
    NetServiceImpl(NetService* service);
    ~NetServiceImpl();

    bool isAvaliable(void);
    bool establishConnection(void);
    bool establishConnectionSync(void);
    bool isConnected(void);

    void readNetConfigs(void);
private:
    void releaseConnection(void);
};

#endif /*_NETWORK_GTK2_H_*/

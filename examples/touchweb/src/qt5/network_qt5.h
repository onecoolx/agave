/* network_qt5.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _NETWORK_QT4_H_
#define _NETWORK_QT4_H_

class NetService;

class NetServiceImpl
{
public:
    NetServiceImpl(NetService*);
    ~NetServiceImpl();

    bool isAvaliable(void);
    bool establishConnection(void);
    bool establishConnectionSync(void);
    bool isConnected(void);

    void readNetConfigs();
private:
    void releaseConnection(void);
};

#endif /*_NETWORK_QT4_H_*/

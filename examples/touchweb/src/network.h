/* network.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <vector>

struct NetConfItemImpl;

struct NetConfItem {
    NetConfItem()
        : isProxy(false)
        , type(0) //default is http
        , impl(0)
    {
    }

    bool isProxy;
    int type;
    ustring name;
    std::string hostip;
    std::string port;
    std::string user;
    std::string pass;
    NetConfItemImpl* impl;
};

class NetServiceImpl;

class NetService
{
public:
    NetService();
    ~NetService();

    bool networkCheck(void);

    bool needProxy(void);

    bool isConnected(void) const;

    bool establishConnection(void);

    bool establishConnectionSync(void);

    bool isAvaliable(void) const { return m_avaliable; }

    void setProxy(const NetConfItem& item);

    void clearProxy(void);

public:
    std::vector<NetConfItem>& getConfigs(void);
private:
    NetServiceImpl* m_impl;
    bool m_avaliable;
    bool m_needProxy;
    std::vector<NetConfItem> m_confs;
};

#endif /*_NETWORK_H_*/

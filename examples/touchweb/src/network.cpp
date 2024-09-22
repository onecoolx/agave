/* network.cpp - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"

#include "network.h"
#include "macross.h"

#if defined(WIN32)
#include "network_win32.h"
#endif

#ifdef GTK2
#include "network_gtk2.h"
#endif

NetService::NetService()
	: m_impl(0)
	, m_avaliable(false)
{
	m_impl = new NetServiceImpl(this);
	m_impl->readNetConfigs();
	m_avaliable = m_impl->isAvaliable();
}

NetService::~NetService()
{
	delete m_impl;
}

std::vector<NetConfItem> & NetService::getConfigs(void)
{
	return m_confs;
}

bool NetService::isConnected(void) const
{
	return m_impl->isConnected();
}

bool NetService::establishConnection(void) 
{
	return m_impl->establishConnection();
}

bool NetService::establishConnectionSync(void) 
{
	return m_impl->establishConnectionSync();
}

bool NetService::networkCheck(void)
{
	if (!m_avaliable)
		return false; //not avaliable

	if (m_impl->isConnected())
		return true;

	if (m_impl->establishConnectionSync()) {
		if (m_impl->isConnected())
			return true;
		else
			return false;
	} else
		return false;
}

bool NetService::needProxy(void)
{
	return m_needProxy;
}

void NetService::setProxy(const NetConfItem& item)
{
	if (item.isProxy) {
		MC_PROXY proxy;
		proxy.Type = PROXY_HTTP;
		proxy.Host = (char*)item.hostip.c_str();
		proxy.Port = (char*)item.port.c_str();
		proxy.UserName = (char*)item.user.c_str();
		proxy.PassWord = (char*)item.pass.c_str();

		macross_set_proxy(&proxy);
		m_needProxy = true;
	}
}

void NetService::clearProxy(void)
{
	macross_set_proxy(0); //disable proxy
	m_needProxy = false;
}

/* network_gtk2.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include "network_gtk2.h"

NetServiceImpl::NetServiceImpl(NetService*)
{
}

NetServiceImpl::~NetServiceImpl()
{
	releaseConnection();
}

bool NetServiceImpl::isAvaliable(void)
{
	return true;
}

bool NetServiceImpl::establishConnection(void)
{
	return true;
}

bool NetServiceImpl::establishConnectionSync(void)
{
	return true;
}

void NetServiceImpl::releaseConnection(void)
{
}

bool NetServiceImpl::isConnected(void)
{
	return true;
}

void NetServiceImpl::readNetConfigs(void)
{
}

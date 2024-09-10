/* network_qt4.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _NETWORK_QT4_H_
#define _NETWORK_QT4_H_

class NetServiceImpl
{
public:
	NetServiceImpl();
	~NetServiceImpl();

	bool isAvaliable(void);
	bool establishConnection(void);
	bool establishConnectionSync(void);
	bool isConnected(void);
private:
	void releaseConnection(void);
};

#endif /*_NETWORK_QT4_H_*/


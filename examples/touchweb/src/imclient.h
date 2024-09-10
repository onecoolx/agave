/* imclient.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _IMCLIENT_H_
#define _IMCLIENT_H_

#include "dashlayer.h"

class ImClient : public DashLayer
{
public:
	ImClient(Widget* parent);
	virtual ~ImClient();

	void setCancel(bool) {}
	void setCancelText(const ustring&) {}
	virtual void Cancel(void);
};

#endif /*_IMCLIENT_H_*/

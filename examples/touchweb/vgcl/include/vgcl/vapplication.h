/**
 *	VApplication.h - Vector graphic component library.
 *
 *	Copyright (C) 2010 Zhang Ji Peng
 *	Contact: onecoolx@gmail.com
 */

#ifndef _VGCL_VAPPLICATION_H_
#define _VGCL_VAPPLICATION_H_

#include "vobject.h"
#include "vcmdtarget.h"

namespace VGCL {

class VApplication : public VCmdTarget
{
public:
	VApplication();
	VApplication(int argc, char** argv);
	virtual ~VApplication();

	void init(int argc, char** argv);

	int run(void);
private:
	VApplication(const VApplication&);
	VApplication& operator=(const VApplication&);
};

}

#endif /*_VGCL_VAPPLICATION_H_*/

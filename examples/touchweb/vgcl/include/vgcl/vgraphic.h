/**
 *	VGraphic.h - Vector graphic component library.
 *
 *	Copyright (C) 2010 Zhang Ji Peng
 *	Contact: onecoolx@gmail.com
 */

#ifndef _VGCL_VGRAPHIC_H_
#define _VGCL_VGRAPHIC_H_

#include "vobject.h"

namespace VGCL {

class VGraphicPrivate;

class VGraphic : public VObject
{
public:
	VGraphic();
	virtual ~VGraphic();

private:
	VGraphicPrivate * m_data;
};

}

#endif /*_VGCL_VGRAPHIC_H_*/

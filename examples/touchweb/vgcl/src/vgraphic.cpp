/**
 *	VGraphic.cpp - Vector graphic component library.
 *
 *	Copyright (C) 2010 Zhang Ji Peng
 *	Contact: onecoolx@gmail.com
 */

#include "vgconfig.h"
#include "vgraphic.h"

#ifdef ENABLE_PICASSO_GRAPHIC
#include "ps/vgraphic_p.h"
#else
#error No valid graphic engine!
#endif

namespace VGCL {

VGraphic::VGraphic()
	: m_data(new VGraphicPrivate)
{
}

VGraphic::~VGraphic()
{
	delete m_data;
}

}

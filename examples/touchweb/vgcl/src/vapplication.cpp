/**
 *	VApplication.cpp - Vector graphic component library.
 *
 *	Copyright (C) 2010 Zhang Ji Peng
 *	Contact: onecoolx@gmail.com
 */

#include "vgconfig.h"
#include "vapplication.h"

namespace VGCL {

VApplication::VApplication()
{
}

VApplication::VApplication(int argc, char** argv)
{
	init(argc, argv);
}

VApplication::~VApplication()
{
}

void VApplication::init(int argc, char** argv)
{
}

int VApplication::run(void)
{
	return 0;
}

}

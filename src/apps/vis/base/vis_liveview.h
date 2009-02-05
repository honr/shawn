/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_VIS_LIVEVIEW_H
#define __SHAWN_VIS_LIVEVIEW_H
#include "../buildfiles/_apps_enable_cmake.h"

#ifdef ENABLE_VIS
#ifdef HAVE_BOOST
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#endif
namespace vis
{
// create window and start window thread:
void createWindow(int sizex, int sizey, int resx, int resy);
// update texture data of window thread with new data created by cairo:
void updateTexture(unsigned char* textureData);
// get pointer to texture array:
unsigned char* getTexture();

#ifdef HAVE_BOOST
boost::mutex* getUpdateMutex();
#endif

}
#endif

#endif

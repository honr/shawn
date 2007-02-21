/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_TOPOLOGY_TOPOLOGY_2D_H
#define __SHAWN_APPS_TOPOLOGY_TOPOLOGY_2D_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/reading/simple_reading.h"

namespace topology
{

   class Topology2D
      : public reading::BoolReading
   {
   public:
      virtual ~Topology2D();
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology/topology_2d.h,v $
 * Version $Revision: 1.3 $
 * Date    $Date: 2006/04/19 11:03:16 $
 *-----------------------------------------------------------------------
 * $Log: topology_2d.h,v $
 *-----------------------------------------------------------------------*/

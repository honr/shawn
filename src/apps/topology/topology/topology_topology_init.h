/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef _SHAWN_APPS_TOPOLOGY_TOPOLOGY_TOPOLOGY_INIT_H
#define _SHAWN_APPS_TOPOLOGY_TOPOLOGY_TOPOLOGY_INIT_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

namespace shawn
{ class SimulationController; }

namespace topology
{

  void init_topology_topology( shawn::SimulationController& );

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology/topology_topology_init.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: topology_topology_init.h,v $
 *-----------------------------------------------------------------------*/

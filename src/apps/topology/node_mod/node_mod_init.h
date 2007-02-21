/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef _SHAWN_APPS_TOPOLOGY_NODE_MOD_NODE_MOD_INIT_H
#define _SHAWN_APPS_TOPOLOGY_NODE_MOD_NODE_MOD_INIT_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

namespace shawn
{ class SimulationController; }

namespace topology
{

  void init_topology_node_mod( shawn::SimulationController& );

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/node_mod/node_mod_init.h,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2005/08/05 10:00:34 $
 *-----------------------------------------------------------------------
 * $Log: node_mod_init.h,v $
 *-----------------------------------------------------------------------*/

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/node_mod/node_mod_init.h"
#include "sys/simulation/simulation_controller.h"

#include <iostream>

namespace topology
{

  void init_topology_node_mod( shawn::SimulationController& sc )
  {
     std::cout << "init_topology_node_mod" << std::endl;


  }

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/node_mod/node_mod_init.cpp,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2005/08/05 10:00:34 $
 *-----------------------------------------------------------------------
 * $Log: node_mod_init.cpp,v $
 *-----------------------------------------------------------------------*/

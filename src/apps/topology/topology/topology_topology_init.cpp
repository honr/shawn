/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/topology/topology_topology_init.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"

#include "apps/topology/topology/rectangle_topology_task.h"
#include "apps/topology/topology/cuboid_topology_task.h"
#include "apps/topology/topology/topology_25d_task.h"
#include "apps/topology/topology/xml_polygon_topology_task.h"
#include "apps/topology/topology/polygon_topology_postscript_task.h"

#include <iostream>

namespace topology
{

  void init_topology_topology( shawn::SimulationController& sc )
  {
     std::cout << "init_topology_topology" << std::endl;

	 sc.simulation_task_keeper_w().add( new RectangleTopologyTask );
     sc.simulation_task_keeper_w().add( new CuboidTopologyTask );
	 sc.simulation_task_keeper_w().add( new Topology25DTask );
	 
	 sc.simulation_task_keeper_w().add( new XMLPolygonTopologyTask );
	 sc.simulation_task_keeper_w().add( new PolygonTopologyPostscriptTask );
  }

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology/topology_topology_init.cpp,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2005/12/02 10:05:26 $
 *-----------------------------------------------------------------------
 * $Log: topology_topology_init.cpp,v $
 *-----------------------------------------------------------------------*/

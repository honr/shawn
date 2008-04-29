/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/elevation/elevation_init.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "apps/topology/elevation/constant_elevation_task.h"
#include "apps/topology/elevation/xyz_file_elevation_task.h"

#include <iostream>

namespace topology
{

  void init_topology_elevation( shawn::SimulationController& sc )
  {
     std::cout << "init_topology_elevation" << std::endl;

     sc.simulation_task_keeper_w().add( new ConstantElevationTask );
     sc.simulation_task_keeper_w().add( new XYZFileElevationTask );
  }

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/elevation/elevation_init.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: elevation_init.cpp,v $
 *-----------------------------------------------------------------------*/

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/generator/generator_init.h"
#include "apps/topology/generator/populate_task.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"

#include <iostream>

namespace topology
{

  void init_topology_generator( shawn::SimulationController& sc )
  {
     std::cout << "init_topology_generator" << std::endl;
     sc.simulation_task_keeper_w().add( new PopulateTask );
  }

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/generator/generator_init.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: generator_init.cpp,v $
 *-----------------------------------------------------------------------*/

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_TOPOLOGY_RECTANGLE_TOPOLOGY_TASK_H
#define __SHAWN_APPS_TOPOLOGY_RECTANGLE_TOPOLOGY_TASK_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"

namespace topology
{

   class RectangleTopologyTask
      : public shawn::SimulationTask
   {
   public:
      RectangleTopologyTask();
      virtual ~RectangleTopologyTask();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

      virtual void run( shawn::SimulationController& sc )
         throw( std::runtime_error );
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology/rectangle_topology_task.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: rectangle_topology_task.h,v $
 *-----------------------------------------------------------------------*/

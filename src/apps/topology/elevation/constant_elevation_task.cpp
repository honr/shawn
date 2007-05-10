/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/elevation/constant_elevation_task.h"
#include "apps/topology/elevation/constant_elevation.h"
#include "apps/topology/topology_keepers.h"
#include "sys/simulation/simulation_environment.h"
#include "apps/reading/reading_keeper.h"

using namespace shawn;
using namespace reading;

namespace topology
{

   ConstantElevationTask::
   ConstantElevationTask()
   {}
   // ----------------------------------------------------------------------
   ConstantElevationTask::
   ~ConstantElevationTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   ConstantElevationTask::
   name( void )
      const throw()
   { return "constant_elevation"; }
   // ----------------------------------------------------------------------
   std::string
   ConstantElevationTask::
   description( void )
      const throw()
   { return "creates a flat elevation at level $height with name $name"; }
   // ----------------------------------------------------------------------
   void
   ConstantElevationTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      double val = sc.environment().required_double_param("height");
      std::string n = sc.environment().optional_string_param("name","");

      elevation_keeper_w(sc).add( new ConstantElevation(val,n) );
   }

}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/elevation/constant_elevation_task.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: constant_elevation_task.cpp,v $
 *-----------------------------------------------------------------------*/

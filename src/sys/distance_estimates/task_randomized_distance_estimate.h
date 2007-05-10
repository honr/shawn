/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_DISTANCE_ESTIMATES_TASK_RANDOMIZED_DISTANCE_ESTIMATE_H
#define __SHAWN_SYS_DISTANCE_ESTIMATES_TASK_RANDOMIZED_DISTANCE_ESTIMATE_H

#include "sys/simulation/simulation_task.h"


namespace shawn
{

   class SimulationTaskRandomizedDistanceEstimate
      : public SimulationTask
   {
   public:
      SimulationTaskRandomizedDistanceEstimate();
      virtual ~SimulationTaskRandomizedDistanceEstimate();
      virtual void run( SimulationController& ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/distance_estimates/task_randomized_distance_estimate.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: task_randomized_distance_estimate.h,v $
 *-----------------------------------------------------------------------*/

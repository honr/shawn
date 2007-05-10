/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_SIMULATION_SIMULATION_TASK_KEEPER_H
#define __SHAWN_SYS_SIMULATION_SIMULATION_TASK_KEEPER_H

#include "sys/simulation/simulation_task.h"
#include "sys/util/handle_keeper.h"

#include <string>
#include <map>

namespace shawn
{

   class SimulationTaskKeeper
      : public HandleKeeper<SimulationTask>
   {
   public:
      SimulationTaskKeeper();
      virtual ~SimulationTaskKeeper();
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/simulation/simulation_task_keeper.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: simulation_task_keeper.h,v $
 *-----------------------------------------------------------------------*/

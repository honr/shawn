/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/simulation/simulation_task_keeper.h"

namespace shawn
{
   SimulationTaskKeeper::
   SimulationTaskKeeper()
      : HandleKeeper<SimulationTask>( "SimulationTaskKeeper", "tasks" )
   {}
   // ----------------------------------------------------------------------
   SimulationTaskKeeper::~SimulationTaskKeeper()
   {}
}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/simulation/simulation_task_keeper.cpp,v $
 * Version $Revision: 1.5 $
 * Date    $Date: 2005/08/05 10:00:42 $
 *-----------------------------------------------------------------------
 * $Log: simulation_task_keeper.cpp,v $
 *-----------------------------------------------------------------------*/

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "exampletask_init.h"
#ifdef ENABLE_EXAMPLETASK

#include "apps/exampletask/exampletask_init.h"
#include "apps/exampletask/exampletask.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"

extern "C" void init_exampletask( shawn::SimulationController& sc )
{
   sc.simulation_task_keeper_w().add( new exampletask::ExampleTask );
}


#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/exampletask/exampletask_init.cpp,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2005/08/05 10:00:29 $
 *-----------------------------------------------------------------------
 * $Log: exampletask_init.cpp,v $
 *-----------------------------------------------------------------------*/

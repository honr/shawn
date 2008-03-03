/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "TraCINodeMovementCreator_init.h"
#ifdef ENABLE_TRACINODEMOVEMENTCREATOR

#include "TraCINodeMovementCreator_init.h"
#include "TraCINodeMovementCreator.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"

extern "C" void init_tracinodemovementcreator( shawn::SimulationController& sc )
{
   sc.simulation_task_keeper_w().add( new traci::TraCINodeMovementCreator(sc) );
}


#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/exampletask/exampletask_init.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: exampletask_init.cpp,v $
 *-----------------------------------------------------------------------*/


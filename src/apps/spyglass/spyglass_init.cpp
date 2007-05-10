/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "spyglass_init.h"
#ifdef ENABLE_SPYGLASS

#include "apps/spyglass/spyglass_init.h"
#include "apps/spyglass/spyglass.h"

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"


extern "C" void init_spyglass( shawn::SimulationController& sc )
{
	sc.simulation_task_keeper_w().add( new spyglass::SpyglassTask );
}


#endif


/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/spyglass/spyglass_init.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: spyglass_init.cpp,v $
 *-----------------------------------------------------------------------*/

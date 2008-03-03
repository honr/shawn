/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2007 by AutoNomos (www.auto-nomos.de)                **
 ** This part of Shawn is free software; you can redistribute it and/or**
 ** modify it under the terms of the BSD License. Refer to the		   **
 ** shawn-licence.txt file in the root of the Shawn source tree for	   **
 ** further details.												   **
 **                                                                    **
 ** \author Axel Wegener <wegener@itm.uni-luebeck.de>				   **
 ** \author Torsten Teubler <teubler@itm.uni-luebeck.de>			   **
 **                                                                    **
 ************************************************************************/

#include "AutoCast_task_init.h"
#ifdef ENABLE_AUTOCAST_TASK

#include "apps/AutoCast/AutoCast_task/AutoCast_task_init.h"
#include "apps/AutoCast/AutoCast_task/AutoCast_task.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"

extern "C" void init_autocast_task( shawn::SimulationController& sc )
{
   std::cout << "AutoCast_task init" << std::endl;
   sc.simulation_task_keeper_w().add( new autocast::AutoCastTask );
}


#endif
/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/


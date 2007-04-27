/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_READING

#include "apps/reading/reading_init.h"
#include "apps/reading/test/test_init.h"

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"

extern "C" void init_reading( shawn::SimulationController& sc )
{   
    reading::init_test_init(sc);   
}

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/reading_init.cpp,v $
 * Version $Revision: 1.3 $
 * Date    $Date: 2006/04/13 17:30:35 $
 *-----------------------------------------------------------------------*/
 

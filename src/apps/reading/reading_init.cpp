/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_READING

#include "apps/reading/reading_init.h"

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "apps/reading/simple_reading_random.h"
#include "apps/reading/test/random_double_test_processor_factory.h"
#include "apps/reading/test/create_random_double_reading.h"
#include <iostream>

extern "C" void init_reading( shawn::SimulationController& sc )
{
   sc.reading_keeper_w().add( new reading::DoubleReadingRandom );
   reading::RandomDoubleTestProcessorFactory::register_factory(sc);
   sc.simulation_task_keeper_w().add( new reading::SimulationTaskReadingDoubleTestCreate );
}

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/reading_init.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------*/


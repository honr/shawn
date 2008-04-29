/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_READING

#include "apps/reading/test/test_init.h"
#include "apps/reading/test/randomheat_task.h"
#include "apps/reading/test/reading_changed_task.h"
#include "apps/reading/test/simple_sensor_task.h"
#include "apps/reading/test/sensor_changed_task.h"
#include "apps/reading/test/xml_reading_parser_task.h"
#include "apps/reading/test/xml_node_reading_parser_task.h"

#include "apps/reading/reading_keeper.h"
#include "apps/reading/simple_sensor.h"
//#include "apps/reading/xml_reading_parser.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"



extern "C" void init_reading_test( shawn::SimulationController& sc )
{   
    //randomheat task initialization
    sc.simulation_task_keeper_w().add ( new reading::RandomHeatTask);
    //readingchanged task initialization
    sc.simulation_task_keeper_w().add ( new reading::ReadingChangedTask);
    //simple_sensor_task initialization
    sc.simulation_task_keeper_w().add ( new reading::SimpleSensorTask);
    //sensor_changed_task initialization
    sc.simulation_task_keeper_w().add ( new reading::SensorChangedTask);
//     //reading_parser_task initialization
//     sc.simulation_task_keeper_w().add ( new reading::XMLReadingParserTask);
//     //node_reading_parser_task initialization
//     sc.simulation_task_keeper_w().add ( new reading::XMLNodeReadingParserTask);    
}

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/test/reading_init.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------*/
 

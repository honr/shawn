/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#ifndef __SHAWN_APPS_SIMPLE_SENSOR_TASK_H
#define __SHAWN_APPS_SIMPLE_SENSOR_TASK_H
 
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_READING
 
#include "apps/reading/simple_sensor.h"
#include "apps/reading/reading_keeper.h"
#include "apps/reading/randomheat_reading.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "sys/node_movements/no_movement.h"
#include "sys/node_movements/linear_movement.h"
#include "sys/node_movements/movement_observer.h"
#include "sys/event_scheduler.h"
#include "sys/misc/box.h"
#include "sys/world.h"
#include "sys/node.h"

namespace reading
 {
 	//class SimpleSensor;
	
	
	class SimpleSensorTask : public shawn::SimulationTask, public shawn::EventScheduler::EventHandler
	{
	public:
		///@name construction/destruction
        	///@{
		SimpleSensorTask();
		///
		virtual ~SimpleSensorTask();
		///@}	
        	///@name basic methods inherited from SimulationTask
        	///@{
        	/**	With this method a name is given to the task. */
		virtual std::string name (void) const throw();
		///
        	/**	With this method a description is given to the task. */	
		virtual std::string description (void) const throw();
		///
        	/**	This method can differ from other implemented tasks.
       	 	*	Basically it is used to run a task. 
        	*/
		virtual void run (shawn::SimulationController& sc) throw (std::runtime_error);
		virtual void timeout( shawn::EventScheduler&, shawn::EventScheduler::EventHandle, double, shawn::EventScheduler::EventTagHandle& ) throw();
		///@}
	private:
		shawn::World* world_;
		reading::IntegerSensor* is_;
	};
};

#endif
#endif	

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/test/simple_sensor_task.h,v $
 * Version $Revision: 1.5 $
 * Date    $Date: 2006/04/19 12:13:47 $
 *-----------------------------------------------------------------------*/


/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_SENSOR_CHANGED_TASK_H
#define __SHAWN_APPS_SENSOR_CHANGED_TASK_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_READING

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "apps/reading/randomheat_reading.h"
#include "apps/reading/reading_keeper.h"
#include "apps/reading/simple_sensor.h"
#include "apps/reading/reading.h"
#include "sys/misc/box.h"
#include "sys/world.h"
#include "sys/node.h"


namespace reading
{
    class SensorChangedTask
        : public shawn::SimulationTask, reading::SensorChangedHandler
    {
    public:
	
        ///@name construction/destruction
        ///@{
        SensorChangedTask();
        ///
        virtual ~SensorChangedTask();
        ///@}	
        ///@name	basic methods inherited from SimulationTask
        ///@{
        /**	With this method a name is given to the task. */
        virtual std::string name( void ) const throw();
        ///
        /**	With this method a description is given to the task. */	
        virtual std::string description( void ) const throw();
        ///
        /**	This method can differ from other implemented tasks.
        *	Basically it is used to run a task. 
	*/
        virtual void run( shawn::SimulationController& sc )
            throw( std::runtime_error );
        ///@}
	///@name basic method inherited from SensorChangedHandler
	///@{
	/**	This method informs the registered SensorChangedHandlers 
	 * 	that a Reading has changed for their particular position.
	 */
	void sensor_changed(reading::Sensor& s, shawn::Vec& v)
	{
		TRACE("I am a sensor changed listener & somebody told me, that there are new things for me to process with");
	};
	///@}
	private:
		shawn::World* world_;
		reading::IntegerSensor* is_;
	};

}

#endif
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/test/sensor_changed_task.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------*/


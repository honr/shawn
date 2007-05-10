/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_RANDOMHEAT_TASK_H
#define __SHAWN_APPS_RANDOMHEAT_TASK_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_READING

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "apps/reading/test/randomheat_task.h"
#include "apps/reading/randomheat_reading.h"
#include "apps/reading/reading_keeper.h"
#include "sys/world.h"

namespace reading
{

    ///	Task that runs the RandomHeatReading
    /**	RandomHeatTask is inherited from the superclass SimulationTask.
    *	It contains basic methods as name() and description() and typical 
    *	for "tasks" a run-method.
    */
    class RandomHeatTask
        : public shawn::SimulationTask
    {
    public:

        ///@name construction/destruction
        ///@{
        RandomHeatTask();
        ///
        virtual ~RandomHeatTask();
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
        *	Basically it is used to run a task. Here, at first the 
        *	RandomHeatReading is found by its name via the "reeding_keeper",
        *	which manages all readings. Afterwards the method "value()" 
        *	from the RandomHeatReading is used to generate random values as
        *	mentioned in the "randomheat_reading.
        */
        virtual void run( shawn::SimulationController& sc )
            throw( std::runtime_error );
        ///@}
    };

}

#endif
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/test/randomheat_task.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: randomheat_task.h,v $
 *-----------------------------------------------------------------------*/


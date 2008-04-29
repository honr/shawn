/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_READINGCHANGED_TASK_H
#define __SHAWN_APPS_READINGCHANGED_TASK_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_READING

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "apps/reading/xml_readings_specifications.h"
#include "apps/reading/randomheat_reading.h"
#include "apps/reading/reading_keeper.h"
#include "sys/world.h"
#include "sys/misc/box.h"

namespace reading
{

    ///	Task that should generate a Reading and initiate an event 
    /**	ReadingChangedTask is inherited from the superclass SimulationTask
     *	and ReadingChangedHandler.
     *	It contains basic methods as name() and description() and typical 
     *	for "tasks" a run-method.
     */
    class ReadingChangedTask
        : public shawn::SimulationTask, reading::ReadingChangedHandler
    {
    public:

        ///@name construction/destruction
        ///@{
        ReadingChangedTask();
        ///
        virtual ~ReadingChangedTask();
        ///@}priovate:	
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
	///@name basic method inherited from ReadingChangedHandler
	///@{
	/** For more information have a look at the documentation about ReadingChangedHandlers*/
	virtual void reading_changed(reading::Reading& r, shawn::Box& b) throw();
	virtual void receiving_settings(xmlreading::Settings&, std::string&) throw();
	virtual void insert_domain_value(const shawn::Vec&, std::string&) throw();
	virtual void erase_domain_value(const shawn::Vec& vector) throw();
	///@}
	
    private:
    	shawn::World* world_;
    };

}

#endif
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/test/reading_changed_task.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------*/


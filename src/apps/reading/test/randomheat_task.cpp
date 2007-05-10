/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_READING

#include "apps/reading/test/randomheat_task.h"

namespace reading
{

    RandomHeatTask::
        RandomHeatTask()
    {}
    // ----------------------------------------------------------------------
    RandomHeatTask::
        ~RandomHeatTask()
    {}
    // ----------------------------------------------------------------------
    std::string
        RandomHeatTask::
        name( void )
        const throw()
    {
        return "randomheat_task";
    }
    // ----------------------------------------------------------------------
    std::string
        RandomHeatTask::
        description( void )
        const throw()
    {
        return "This task uses a reading of the type integer. It is used to display 'randomized' integer values. Furthermore the 'randomheat_reading' is able to throw time events. For this, the 'randomheat_reading' has been derived from the class EventHandler. And the actual time event is initialized in the readings init()-method. The init()-method is no precondision in throwing events. As described above, you just have to derive your classes from the 'EventHandler' class. If you want to activate this task, you will just have to uncomment the init()-method in the randomheat_reading.h & .cpp. and call the task 'randomheat_task' by its name while running shawn.";
    }
    // ----------------------------------------------------------------------
    void
        RandomHeatTask::
        run( shawn::SimulationController& sc )
        throw( std::runtime_error )
    {
	sc.reading_keeper_w().add( new reading::RandomHeatReading(sc));
	
	reading::ReadingKeeper::Handle rh = sc.reading_keeper_w().find_w("randomheat_reading");
        if( !rh.is_null() )
        {
            reading::IntegerReading* ir = dynamic_cast<reading::IntegerReading*> (rh.get());
	    
            if( ir != NULL )
            {
                for(int i = 0; i < 100; ++i)
                {
                    shawn::Vec v(i, i, i);
                    std::cout << "Heat reading at " << v << " is " << ir->value(v) << std::endl;
                }

            }
        }
    }

}

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/test/randomheat_task.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: randomheat_task.cpp,v $
 *-----------------------------------------------------------------------*/


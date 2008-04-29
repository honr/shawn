/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

 #include "_apps_enable_cmake.h"
 #ifdef ENABLE_READING
 
#include "apps/reading/test/reading_changed_task.h"
 
 namespace reading
 {
  
     ReadingChangedTask::
         ReadingChangedTask()
     {}
     // ----------------------------------------------------------------------
     ReadingChangedTask::
         ~ReadingChangedTask()
     {}     
     // ----------------------------------------------------------------------
     std::string
         ReadingChangedTask::
         name( void )
         const throw()
     {
         return "reading_changed_task";
     }
     // ----------------------------------------------------------------------
     std::string
         ReadingChangedTask::
         description( void )
         const throw()
     {
         return "This is a task used to demonstrate, how to add 'ReadingChangedHandlers' to a 'reading' and how the listeners are notified about changes in the world. In better terms: The reading holds information about the world, e.G. temperature values. These values can change over time, therefore listeners like sensors have to be informed about these changes. This is all done by this task.";
     }
     // ----------------------------------------------------------------------
     void
         ReadingChangedTask::
         run( shawn::SimulationController& sc )
         throw( std::runtime_error )
     {
         TRACE(description());
	 require_world( sc );
	 world_ = &sc.world_w();
	 assert ( world_ != NULL);
	 
	 sc.reading_keeper_w().add( new reading::RandomHeatReading(sc));
	 reading::ReadingKeeper::Handle rh = sc.reading_keeper_w().find_w("randomheat_reading");
         rh->init();
	 TRACE("READING_CHANGED_TASK: search for randomheat reading")
	 
	 if( !rh.is_null() )
         {
              TRACE("READING_CHANGED_TASK:	randomheat reading found");
	      TRACE("READING_CHANGED_TASK: generating new boxes for examination purposes");
	      shawn::Box b_1(shawn::Vec(10,10,10), shawn::Vec(20,20,20));
	      TRACE("READING_CHANGED_TASK: b_1((10,10,10),(20,20,20))");
	      shawn::Box b_2(shawn::Vec(100,100,100), shawn::Vec(20,20,20));
	      TRACE("READING_CHANGED_TASK: b_2((100,100,100),(20,20,20))");
	      shawn::Box b_3(shawn::Vec(3,5,100), shawn::Vec(9,10,10));
	      TRACE("READING_CHANGED_TASK: b_3((3,5,100),(9,10,10))");
	      	      
	      TRACE("READING_CHANGED_TASK: filling vector ReadingChangedHandlerList with <ReadingChangedHandler, Box>-pairs");
	      
	      rh->add_changed_handler(this, b_1);
              rh->add_changed_handler(this, b_2);
	      rh->add_changed_handler(this, b_3);
	      
	      TRACE("	done");
	 }   
    }
    // ----------------------------------------------------------------------
    void ReadingChangedTask::reading_changed(reading::Reading& r, shawn::Box& b) throw()
    {
    	TRACE("READING_CHANGED_TASK: got new values...need to inform all listeners");
	
    }
    // ----------------------------------------------------------------------
    void ReadingChangedTask::receiving_settings(xmlreading::Settings& settings, std::string& reading_data_type) throw(){}
    // ----------------------------------------------------------------------
    void ReadingChangedTask::insert_domain_value(const shawn::Vec& vector, std::string& value) throw(){}
    // ----------------------------------------------------------------------
    void ReadingChangedTask::erase_domain_value(const shawn::Vec& vector) throw(){};
    // ----------------------------------------------------------------------
}
 
#endif
 
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/test/reading_changed_task.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------*/


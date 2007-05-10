/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
  
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef	ENABLE_READING 

#include "apps/reading/randomheat_reading.h"
  
 using namespace shawn;
 
 namespace reading
 {
     RandomHeatReading::
          
	 RandomHeatReading(shawn::SimulationController& sc) : sc_(&sc)
     {}
     // ----------------------------------------------------------------------
     RandomHeatReading::
         ~RandomHeatReading()
     {}
     // ----------------------------------------------------------------------
     std::string
         RandomHeatReading::
         name( void )
         const throw()
     { return "randomheat_reading"; }
 
     // ----------------------------------------------------------------------
     std::string 
         RandomHeatReading::
         description( void )
         const throw()
     {
         return "An integer reading. Taking a closer look at this reading you will determine, that this is not a reading where randomized values are generated. This is done by the 'randomheat_task'. Furthermore this reading is able to initialize time events by deriving itself from the 'EventHandler' class. For more details read the documentation.";
     }
     // ----------------------------------------------------------------------
     IntegerReading::ValueType 
         RandomHeatReading::
         value(const Vec& v ) 
         const throw()
     {
         return rand() % 40;
     }
     // ----------------------------------------------------------------------
	void RandomHeatReading::init( void) throw()
     {
	//assert( world_ != NULL );
	TRACE("RANDOMHEAT_READING: initialize new event");
	event_handle_ = sc_->world_w().scheduler_w().new_event(*this, 4.1, NULL);
	TRACE("RANDOMHEAT_READING:	initialization completed");
     }
     // ----------------------------------------------------------------------
     void RandomHeatReading::timeout(shawn::EventScheduler& event_scheduler, shawn::EventScheduler::EventHandle event_handle, double time, shawn::EventScheduler::EventTagHandle& event_tag_handle) throw()
     {
	int count = 0;
	TRACE("RANDOMHEAT_READING: here we assume, that the reading has new values for this box");
	shawn::Box b_(shawn::Vec(3,5,100), shawn::Vec(10,10,10)); 
	TRACE("RANDOMHEAT_READING: time: "<< time << "   reading has changed in box((3,5,100), (10,10,10))");
	
	TRACE("RANDOMHEAT_READING: search for intersecting boxes");
	TRACE("if the box with the new values intersects with other boxes, they also have to be informed about new values") 
	for(ReadingChangedHandlerList::iterator it = rbv.begin(); it != rbv.end(); ++it)
	{
		count++;
		TRACE("box: " << count);
		
		if( b_.intersects(it->second) )
		{
			TRACE("RANDOMHEAT_READING: intersection found");
			TRACE("RANDOMHEAT_READING:	get corresponding ReadingChangedHandler (listener) object");
			reading::ReadingChangedHandler* rch_ = (it)->first;
			TRACE("RANDOMHEAT_READING:	reading_changed() for this listener");
			rch_->reading_changed(*this, it->second);   
		}
		else
		{
			TRACE("	nothing to do");
		}
	} 
     }
     
   
     
 }

#endif 
 
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/reading/randomheat_reading.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: randomheat_reading.cpp,v $
 *-----------------------------------------------------------------------*/



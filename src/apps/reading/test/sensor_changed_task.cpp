/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

 #include "_apps_enable_cmake.h"
 #ifdef ENABLE_READING
 
 #include "apps/reading/test/sensor_changed_task.h"
   
 namespace reading
 {
  
     SensorChangedTask::
         SensorChangedTask() : world_ (NULL), is_ (NULL)
     {}
     // ----------------------------------------------------------------------
     SensorChangedTask::
         ~SensorChangedTask()
     {
     	delete is_;
	is_ = NULL;
     }     
     // ----------------------------------------------------------------------
     std::string
         SensorChangedTask::
         name( void )
         const throw()
     {
         return "sensor_changed_task";
     }
     // ----------------------------------------------------------------------
     std::string
         SensorChangedTask::
         description( void )
         const throw()
     {
         return "This is a task, which should pass through information from a reading to a sensor and then to a 'sensor changed handler'. All this should happen on a timeout signal emitted by the 'randomheat_reading'. Take a look at the  randomheat_reading's init()-method and check if it's uncommented. Otherwise the Task will not work!";
     }
     // ----------------------------------------------------------------------
     void
         SensorChangedTask::
         run( shawn::SimulationController& sc )
         throw( std::runtime_error )
     {
     	 TRACE(description());
	 require_world( sc );
	 world_ = &sc.world_w();
	 assert(world_->node_count()>0);
	 shawn::Node& n = *(world_->begin_nodes_w());
	 
	 sc.reading_keeper_w().add( new reading::RandomHeatReading(sc));
	 
	 TRACE("search for randomheat reading");
	 reading::ReadingKeeper::Handle rh = sc.reading_keeper_w().find_w("randomheat_reading");
         TRACE("     done");
	 reading::IntegerReading* ir = dynamic_cast<reading::IntegerReading*>(rh.get());
	 ir->init();
	 TRACE("creating new integer sensor");
	 is_ = new reading::IntegerSensor(n, *ir);
	 TRACE("     done");
	 
	 if( !rh.is_null() )
         {
              
	      TRACE("generating new box for examination purposes");
	      shawn::Box b_1(shawn::Vec(3,5,100), shawn::Vec(10,10,10));
	      TRACE("b_1((3,5,100),(10,10,10)))");
	      TRACE("     done");
	      	      
	      TRACE("filling vector RBVec with <ReadingChangedHandler, Box>-pairs, here just one integer sensor");
	      rh->add_changed_handler(is_, b_1);
              
	      TRACE("     done");
	      
	      TRACE("filling vector 'SensorChangedHandlerVector' with sensor listeners");
	      is_->add_sensor_changed_handler(this);
	      TRACE("     done"); 
	 }   
     }
    // ----------------------------------------------------------------------

 }
 
 #endif
 
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/test/sensor_changed_task.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: sensor_changed_task.cpp,v $
 *-----------------------------------------------------------------------*/

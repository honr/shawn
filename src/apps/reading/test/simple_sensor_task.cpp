/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"  
#ifdef ENABLE_READING

#include "apps/reading/test/simple_sensor_task.h"

namespace reading
{
	SimpleSensorTask::SimpleSensorTask() : world_ (NULL), is_ (NULL){}
	//--------------------------------------------------------------
	SimpleSensorTask::~SimpleSensorTask()
	{
		delete is_;
		is_ = NULL;
	}
	//--------------------------------------------------------------
	std::string SimpleSensorTask::name(void) const throw()
	{
		return "simple_sensor_task";
	}
	//--------------------------------------------------------------
	std::string SimpleSensorTask::description(void) const throw()
	{
		return "This is a task, which shows you how to initialize a node movement and the notification of movement observers. Here we use a sensor. When the node leaves the listeners domain, e.G. the sensor range (Box). The listeners have to be informed about the new location of the node to ensure, that the sensors deliver correct values. ";
	}
	//--------------------------------------------------------------
	void SimpleSensorTask::run(shawn::SimulationController& sc)
	throw (std::runtime_error)
	{
		READING_TRACE(description());
		
		sc.reading_keeper_w().add( new reading::RandomHeatReading(sc));
		
		require_world(sc);
		world_ = &sc.world_w();
		assert(world_->node_count()>0);
		
		shawn::Vec gran(20,20,20);
		shawn::Vec v(1,1,1);
				
		shawn::Node& n = *(world_->begin_nodes_w());
		
		reading::ReadingKeeper::Handle rh = sc.reading_keeper_w().find_w("randomheat_reading");
		
		reading::IntegerReading* ir = dynamic_cast<reading::IntegerReading*> (rh.get());
		
		READING_TRACE("SimpleSensor declaration, in this case an IntegerSensor");
		is_ = new reading::IntegerSensor(n, *ir);
		is_->set_granularity(gran);
		
		if(world_ ->simulation_round() == 0)
		{
			READING_TRACE("Initialize new node movement");
			shawn::NoMovement* jm = new shawn::NoMovement();
			jm->set_position(v);
			n.set_movement(jm);
			
			READING_TRACE("current position of node n " << n.real_position());
			READING_TRACE("current box of the sensor is: " << is_->get_updated_zone());
			
			
		}
		else if (world_ ->simulation_round() == 1)
		{
			v = shawn::Vec(200, 0, 0);
			shawn::LinearMovement* jm = new shawn::LinearMovement();
			jm->set_parameters(10, v, *world_);
			n.set_movement(jm);
			
			READING_TRACE("current position of node n " << n.real_position());
			READING_TRACE("destination position: 	   " << v);
			READING_TRACE("current box of the sensor is: "<< is_->get_updated_zone());
			
			
		} 
		else
		{
			READING_TRACE("current position of node n " << n.real_position());
			READING_TRACE("current box of the sensor is: "<< is_->get_updated_zone());
		}
	}
	//-------------------------------------------------------------
	void SimpleSensorTask::timeout(shawn::EventScheduler& event_scheduler, shawn::EventScheduler::EventHandle event_handle, double time, shawn::EventScheduler::EventTagHandle& event_tag_handle) throw()
	{
		shawn::Node& n = *(world_->begin_nodes_w());
		READING_TRACE("SimpleSensorTask::timeout(time:"<<time<<"):pos : " << n.real_position());
	}	
}

#endif   

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/test/simple_sensor_task.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------*/


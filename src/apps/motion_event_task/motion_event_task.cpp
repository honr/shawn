/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_MOTIONEVENTTASK

#include "apps/motion_event_task/motion_event_task.h"
#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/event_scheduler.h"
#include "sys/vec.h"
#include "sys/misc/random/normal_random_variable.h"
#include "sys/misc/random/random_variable_keeper.h"

#include "sys/taggings/basic_tags.h"
#include "sys/taggings/map_tags.h"
#include "sys/misc/tokenize.h"
#include "sys/util/string_conv.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;
using namespace shawn;

#define MAX_ERROR 10000000.0

namespace motion_event
	{

	MotionEventTask::
	MotionEventTask()
	{
		object_id_ = 0;
		velocity_ = 1.0;
		detection_range_ = 1.0;
		start_time_ = 0.0;
		standard_deviation_ = 0.0;
		max_time_error_ = MAX_ERROR;
	}
	// ----------------------------------------------------------------------
	MotionEventTask::
	~MotionEventTask()
	{}
	// ----------------------------------------------------------------------
	std::string
	MotionEventTask::
	name( void )
	const throw()
	{
		return "motion_event_task";
	}
	// ----------------------------------------------------------------------
	std::string
	MotionEventTask::
	description( void )
	const throw()
	{
		return "A generator for motion events in the world";
	}
	// ----------------------------------------------------------------------
	void
	MotionEventTask::
	run( shawn::SimulationController& sc )
	throw( std::runtime_error )
	{
		require_world( sc );

		object_id_ = sc.environment().required_int_param("object_id");
		velocity_ = sc.environment().required_double_param("velocity");
		detection_range_ = sc.environment().required_double_param("detection_range");
		start_time_ = sc.environment().required_double_param("start_time");
		standard_deviation_ = sc.environment().optional_double_param("standard_deviation", 0.0);
		max_time_error_ = sc.environment().optional_double_param("max_time_error", MAX_ERROR);

		for (shawn::World::node_iterator iter = sc.world_w().begin_nodes_w(); iter != sc.world_w().end_nodes_w(); ++iter) 
		{
			pair<shawn::Node*, bool> p;
			p.first = &(*iter);
			p.second = false;
			in_range_of_nodes_.push_back(p);
		}

		// Reading of positions (vectors) of the object
		StrTok tok(sc.environment().required_string_param("object"), ",;() ");
		StrTok::iterator it = tok.begin();
		for(int i=0; i<tok.size()-3; i+=3)
		{
			double x_pos = conv_string_to_double( *it );
			it++;
			double y_pos = conv_string_to_double( *it );
			it++;
			double z_pos = conv_string_to_double( *it );
			it++;		
			double x_dest = conv_string_to_double( *it );
			it++;
			double y_dest = conv_string_to_double( *it );
			it++;
			double z_dest = conv_string_to_double( *it );
			it-=2;	

			Vec position = Vec( x_pos, y_pos, z_pos );
			Vec destination = Vec( x_dest, y_dest, z_dest );
			single_line(sc, position, destination);
		}

	}
	// ----------------------------------------------------------------------
	void 
	MotionEventTask::
	single_line(shawn::SimulationController& sc, const shawn::Vec start_pos, const shawn::Vec dest_pos) 
	{
		Vec direction = dest_pos - start_pos;
		double dir_norm = direction.euclidean_norm();
		for( shawn::World::node_iterator it = sc.world_w().begin_nodes_w(); it != sc.world_w().end_nodes_w(); ++it )
		{
			shawn::Node& node = *it;
			//cout << "Node: " << it->id() << endl;
			Vec n = node.real_position();

			//Minimum distance between node and moving event
			double distance = ((cross_product((n - start_pos), direction)).euclidean_norm())/dir_norm;

			//cout << "Distance: " << distance << endl;
			if (distance <= detection_range_) 
			{
				//computation of node projection on the line of the moving event (between pos_ and dest_)
				double lambda = ( (n - start_pos) * direction ) / (dir_norm * dir_norm) ;
				Vec intercept = start_pos + (direction * lambda);

				//cout << "Nod_pos: " << n.x() << "," << n.y() << "," << n.z() << endl;
				//cout << "Dest: " << dest_pos.x() << "," << dest_pos.y() << "," << dest_pos.z() << endl;
				//cout << "Distance: " << euclidean_distance(n, dest_pos) << " <= " << detection_range_ << endl;
				
				//If (the projection is between pos_ and dest_ or if the person is near enough) and the person enters the detection_range
				//the node remarks the event
				if ( ( between(intercept, start_pos, dest_pos) || 
				(euclidean_distance(n, start_pos) <= detection_range_) || 
				(euclidean_distance(n, dest_pos) <= detection_range_) ) && 
				!in_range_of_node(&node) ) 
				{
					// If person does not leave the detection range of the node set in_range_of_node = true
					if ( euclidean_distance(n, dest_pos) <= detection_range_ ) 
						set_in_range_of_node(&node, true);
					else set_in_range_of_node(&node, false);

					//compute the time, at which the node remarks the motion event
					NormalRandomVariable* var = new NormalRandomVariable;
					var->set_standard_deviation(standard_deviation_);
					var->init();
					double random_variable = *var;
					while (random_variable < (-1.0)*max_time_error_ || random_variable > max_time_error_)
						random_variable = *var;

					double intercept_time = start_time_ + random_variable + euclidean_distance(start_pos, intercept)/velocity_;

					delete var;

					cout << "Person " << object_id_ << " hits node " << node.id() << " at position " << intercept << " at " << intercept_time << endl;

					// Saving event information in a tag
					TagHandle th = node.find_tag_w("MotionEventTag");
					shawn::IntegerDoubleMapTag* mt;
					if ( th.is_not_null() )
						mt = dynamic_cast<IntegerDoubleMapTag*>(th.get());
					else mt = new shawn::IntegerDoubleMapTag("MotionEventTag");
					{
						const IntegerDoubleMapTag::Map& m = mt->value();
						std::ostringstream ossint, ossdouble;
						ossint << m.size();
						ossdouble << intercept_time;
						
						mt->add_indexed_entry( ossint.str(), ossdouble.str());
						mt->set_persistency(true);
						node.add_tag(mt);
					}
				}
				else set_in_range_of_node(&node, false);
			}
		}
		start_time_ += euclidean_distance(start_pos, dest_pos)/velocity_;
	}
	// ----------------------------------------------------------------------
	bool
	MotionEventTask::
	between(const Vec r, Vec p, Vec q) 
	{
		bool b = true;
		if (!((p.x() <= r.x() && r.x() <= q.x()) || (q.x() <= r.x() && r.x() <= p.x()))
		|| !((p.y() <= r.y() && r.y() <= q.y()) || (q.y() <= r.y() && r.y() <= p.y()))
		|| !((p.z() <= r.z() && r.z() <= q.z()) || (q.z() <= r.z() && r.z() <= p.z())))
			b = false;
		return b;
	}
	// ----------------------------------------------------------------------
	bool
	MotionEventTask::
	in_range_of_node(shawn::Node* n)
	{
		for (std::list<std::pair<shawn::Node*,bool> >::iterator it = in_range_of_nodes_.begin(); it != in_range_of_nodes_.end(); it++)
		{
			if ((*it).first == n)
			return (*it).second;
		}
		return false;
	}// ----------------------------------------------------------------------
	void
	MotionEventTask::
	set_in_range_of_node(shawn::Node* n, bool b)
	{
		//cout << "in range of node " << n->id() << " = " << b << endl;
		for (std::list<std::pair<shawn::Node*,bool> >::iterator it = in_range_of_nodes_.begin(); it != in_range_of_nodes_.end(); it++)
		if ((*it).first == n)
		(*it).second = b;
	}


	}
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/tubsapps/motion_event_task/motion_event_task.cpp,v $
* Version $Revision: 1.2 $
* Date    $Date: 2007/01/31 17:24:16 $
*-----------------------------------------------------------------------
* $Log: motion_event_task.cpp,v $
* Revision 1.2  2007/01/31 17:24:16  krueger
* *** empty log message ***
*
* Revision 1.1  2007/01/29 16:37:14  krueger
* *** empty log message ***
*
* Revision 1.1  2005/03/05 06:43:10  krueger
* added motion_event_task
*
*-----------------------------------------------------------------------*/


/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_LEGACYAPPS_TRACINODEMOVEMENTCREATOR_H
#define __SHAWN_LEGACYAPPS_TRACINODEMOVEMENTCREATOR_H

#include "../buildfiles/_apps_enable_cmake.h"
//#include "sys/event_scheduler.h"
#include "sys/node_movements/playback/movement_info.h"
#include "sys/node_movements/playback/node_movement_creator.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "sys/node_movement.h"
#include "sys/node.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/processors/processor_factory.h"
#include "sys/node_movements/playback/movement_info.h"
#include "sys/vec.h"
#include "apps/tcpip/socket.h"
#include "apps/tcpip/storage.h"
#include "mobilityInterfaceConstants.h"
#include <string>
#include <list>
#include <map>
#ifdef ENABLE_TRACINODEMOVEMENTCREATOR

namespace traci
{
	class TraCINodeMovementCreator :
		public shawn::NodeMovementCreator,
		public shawn::SimulationTask
	{
	public:
		TraCINodeMovementCreator(shawn::SimulationController& sc);
		~TraCINodeMovementCreator();

		std::string name( void ) const throw();

		std::string description( void ) const throw();

		shawn::MovementInfo* next_movement(void) throw( std::runtime_error );

		void run(shawn::SimulationController&) throw( std::runtime_error );
	
		void reset();

		void command_set_maximum_speed(shawn::Node& node, double max_speed);
		void command_stop(shawn::Node& node, double x, double y, double radius, double wait_time);
		void command_change_route(shawn::Node& node, std::string road_id, double travel_time);
		void command_change_lane(shawn::Node& node, char lane, float time);

	private:

		// Reads a Status command that is expected to start at s' actual position.
        // If the status is unequal to RTYPE_OK or the given commandId differs from
        // that in the Storage, Shawn will stop.
		void extract_command_status(tcpip::Storage& s, unsigned char commandId, std::string& description);

		// Connect and disconnect to the mobility server         
		// The connect method is called by startSimStepHandler         
		// The close method is called by the destructor
		bool connect();
		void close();

		void fetch_processor_factories();

		//void switch_processors_state(int id = -1, int state = 0);

		//bool all_processors_state(int node_id, int state);

		void check_for_unused_nodes();

		void increase_nodes(int to);

		shawn::Node* new_node();

		// Internal variables
		shawn::SimulationController &sc_;
		tcpip::Socket * socket_;
		tcpip::Storage in_;
		std::string remotehost_;
		int remoteport_;
		double time_interval_;
		double target_time_;
		shawn::MovementInfo* mi_;
		std::list<shawn::ProcessorFactoryHandle> processor_factories_;
		std::set<int> current_nodes_;

   };
}
#endif
#endif

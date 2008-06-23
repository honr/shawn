/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_NODE_MOVEMENTS_PLAYBACK_RANDOM_DIRECTION_NODE_MOVEMENT_CREATOR
#define __SHAWN_SYS_NODE_MOVEMENTS_PLAYBACK_RANDOM_DIRECTION_NODE_MOVEMENT_CREATOR

#include "sys/event_scheduler.h"
#include "sys/node_movements/playback/movement_info.h"
#include "sys/node_movements/playback/node_movement_creator.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/node_movement.h"
//#include "sys/taggings/group_tag.h"
#include "sys/node.h"
#include <string>
#include <map>
#include "sys/simulation/simulation_environment.h"
#include "sys/misc/random/uniform_random_variable.h"


namespace shawn
{
	class RandomDirectionNodeMovementCreator:
		public NodeMovementCreator
	{
		public:
      ///@name construction / destruction
      ///@{
      ///
      RandomDirectionNodeMovementCreator(SimulationController& sc);

		~RandomDirectionNodeMovementCreator();
		      ///@}


      ///@name methods inherited from NodeMovementCreator
      ///@{
      /** This method scans all Nodes regarding the next Movement from the xml tags belonging 
		 *  to distinct nodes.
		 *
       */

		virtual shawn::MovementInfo *next_movement(void) throw( std::runtime_error );
	
		/** This method causes the NodeMovementCreator to start movements
       *  from the beginning.
       *
       */
		virtual void reset();

	private:
      MovementInfo* generateNewMovement(Node &node, double startTime);

      SimulationController &sc_;
      
      // Map to save times, when nodes will fall from the border of the world
      std::multimap<double, Node*> next_movement_times_;

      // Random Variable for getting speeds
      shawn::UniformRandomVariable urvSpeed_;
      // Random Variable for getting directions
      shawn::UniformRandomVariable urvDirection_;
      
      // The world does not know its maximum dimension, so get it from the SimulationController
      double width_;
      double height_;
   };
}
#endif

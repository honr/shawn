/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "sys/world.h"
#include "sys/node_movements/playback/movement_controller.h"
#include "sys/event_scheduler.h"
#include "sys/node_movements/playback/movement_info.h"
#include <cmath>



namespace shawn
{
    MovementController::MovementController(SimulationController& sc) :
      sc_(&sc),
      nmc_(NULL),
      movement_info_(NULL),
      command_nr_(0)
   {
   }

   // ----------------------------------------------------------------------

   MovementController::~MovementController()
   {
      if (nmc_!= NULL)
         delete nmc_;
   }

   // ----------------------------------------------------------------------

   void MovementController::timeout (EventScheduler & event_scheduler, EventScheduler::EventHandle event_handle, double time, EventScheduler::EventTagHandle & event_tag_handle) throw()
   {
      timeout_action();
   }

   // ----------------------------------------------------------------------

   void MovementController::timeout_action()
   {
		

      if (movement_info_ != NULL)
      {
         DEBUG(sc_->logger(), "MovementController::timeout_action() setting a delayed node movement");
			NodeMovement* n =movement_info_->node_movement();
         movement_info_->node()->set_movement(movement_info_->node_movement());
         command_nr_++;
      }
		movement_info_ = nmc_->next_movement();
	      
		if (movement_info_ != NULL)
         {

// if ( movement_info_->node()->id() == 0 )
// {
//    WARN( sc_->logger(), "It's 0 that is going to be at " << movement_info_->node_movement()->position() );
//    WARN( sc_->logger(), "Before at " << movement_info_->node()->real_position() );
// }
         
         switch(movement_info_->urgency())
            {
            case MovementInfo::Immediately:
               DEBUG(sc_->logger(), "MovementController::timeout_action() - Immediately");
               (movement_info_->node())->set_movement(movement_info_->node_movement());
               command_nr_++;

// if ( movement_info_->node()->id() == 0 )
// {
//    WARN( sc_->logger(), "And after (still) at " << movement_info_->node()->real_position() );
//    WARN( sc_->logger(), "----" );
// }
               
               movement_info_ = NULL;
               timeout_action();
            break;
            case MovementInfo::Delayed:
               if (sc_->world_w().current_time() < movement_info_->dispatch_time())
               {
                  DEBUG(sc_->logger(), "MovementController::timeout_action() - Delayed");
                  event_handle_ = sc_->world_w().scheduler_w().new_event(*this, movement_info_->dispatch_time(),NULL);
                  command_nr_++;

// if ( movement_info_->node()->id() == 0 )
// {
//    WARN( sc_->logger(), "Delayed" );
//    WARN( sc_->logger(), "----" );
// }
                  
               }
               else
               if EQDOUBLE(sc_->world_w().current_time(),movement_info_->dispatch_time())
               {
                  DEBUG(sc_->logger(), "MovementController::timeout_action() - Immediately (ftime=stime)");
                  (movement_info_->node())->set_movement(movement_info_->node_movement());
                  command_nr_++;

// if ( movement_info_->node()->id() == 0 )
// {
//    WARN( sc_->logger(), "And after (still) at " << movement_info_->node()->real_position() );
//    WARN( sc_->logger(), "----" );
// }
                  
                  movement_info_ = NULL;
                  timeout_action();
               }
               else
               {
                  WARN(sc_->logger(), "Illegal NodeMovement scheduling time found in delayed command nr. "<<command_nr_);
                  command_nr_++;
                  movement_info_ = NULL;
                  timeout_action();
               }
            break;
            }

         }

   }

   // ----------------------------------------------------------------------

   void MovementController::set_node_movement_creator(NodeMovementCreator* nmc)
   {
      nmc_ = nmc;
   }

   // ----------------------------------------------------------------------

   void MovementController::start()
   {
      if (&nmc_ != NULL)
      {
         command_nr_=0;
         timeout_action();
      }
      else
      {
         ERROR(sc_->logger(), "MovementController.start() called but MC has no valid NodeMovementCreator. No NodeMovement will take place.");
      }
   }

   // ----------------------------------------------------------------------

   void MovementController::reset()
   {
      if (&nmc_ != NULL)
      {
         nmc_->reset();
         command_nr_=0;
         timeout_action();
      }
   }
}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/node_movements/playback/movement_controller.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
* $Log: movement_controller.cpp,v $
 *-----------------------------------------------------------------------*/

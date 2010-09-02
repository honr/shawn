/************************************************************************
 ** Source  $HeadURL: https://www.ldv.ei.tum.de/repos/vertrauenstuds/shawn-apps/trunk/sumo/sumo_node_movement.cpp $
 ** Version $Revision: 416 $
 ** Id      $Id: sumo_node_movement.cpp 416 2010-09-01 22:12:09Z wbam $
 ************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2010 Josef Schlittenlacher                           **
 ** Copyright (C) 2010 Technische Unversitaet Muenchen (www.tum.de)    **
 ** All rights reserved.                                               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sumo_node_movement.h"
#ifdef ENABLE_SUMO

#include "sys/node_movements/jump_movement.h"
#include "sys/node_movements/linear_movement.h"
#include "sys/processors/processor_keeper.h"
#include "sys/misc/tokenize.h"
#include "sys/transmission_model.h"

using namespace std;

namespace shawn
{

SumoNodeMovementCreator::SumoNodeMovementCreator( SimulationController& sc ) :
      sc_( &sc ),
      last_round_( 0 ),
      last_parsed_round_( 0 ),
      net_( sc_->environment_w().required_string_param( "net_file" ) )
{
   // Open the dump file for input
   dump_file_ = sc_->environment_w().required_string_param( "dump_file" );
   ifs_dump_file_.open( dump_file_.c_str() );																			// dumpfile to input stream

   // Create the buffers for the entries in the dump file. Current positions become to last positions in the next step. Because of this reason, we need pointers.
   map_current_positions_ = new map<string, Vec>();
   map_last_positions_ = new map<string, Vec>();

   // Generate the set of processor factories as configured by the user
   StrTok processors_tok_( sc_->environment_w().optional_string_param( "processors", "" ), "," );
   for ( StrTok::iterator iter = processors_tok_.begin(); iter != processors_tok_.end(); ++iter )
   {
      ProcessorFactoryHandle pfh = sc_->processor_keeper_w().find_w( *iter );
      assert( pfh != NULL );
      processor_factories_.push_back( pfh );
   }

   // A dummy node is needed so that at least 1 active node always exists.
   // (A sumo dump file need not start with an existing node.)
   // TODO: This is a hack. Suggest another solution to the Shawn list.
   create_dummy_node();
}

// -----------------------------------------------------------------------------

SumoNodeMovementCreator::~SumoNodeMovementCreator()
{
   delete map_current_positions_;
   delete map_last_positions_;
}

// -----------------------------------------------------------------------------

MovementInfo * SumoNodeMovementCreator::next_movement()
{
   int current_round = sc_->world_w().simulation_round();
   if ( current_round > last_round_ )											// next_movement() is executed the first time at this round
   {
      next_round();
      last_round_ = current_round;
   }
   MovementInfo * movement_info = NULL;

   if ( remaining_movements_.size() > 0 )									// return next movement from remaining movement vector
   {
      movement_info = remaining_movements_.back();
      remaining_movements_.pop_back();
   }
   else																										// no more movement this round, return zero-node-movement
   {
      movement_info = new MovementInfo();
      movement_info->set_urgency( MovementInfo::Delayed );
      if ( current_round >= 0 )
         movement_info->set_dispatch_time( static_cast<double>( current_round ) + 1.0 );
      else
         movement_info->set_dispatch_time( 1.0 );
      movement_info->set_node_movement( NULL );
      movement_info->set_node( NULL );
   }
   return movement_info;
}

// -----------------------------------------------------------------------------

void SumoNodeMovementCreator::reset()
{
   ifs_dump_file_.close();
   ifs_dump_file_.open( dump_file_.c_str() );

   map_current_positions_ = new map<string, Vec>();
   delete map_current_positions_;

   map_last_positions_ = new map<string, Vec>();
   delete map_last_positions_;
}

// -----------------------------------------------------------------------------

void SumoNodeMovementCreator::next_round()
{
   round_data_from_dumpfile();
   add_new_nodes();
   delete_nodes();
   generate_movements();
}

// -----------------------------------------------------------------------------

void SumoNodeMovementCreator::round_data_from_dumpfile()
{
   // Throw away data of second last round,
   // save positions of last round, and
   // create new container for new positions.
   delete map_last_positions_;
   map_last_positions_ = map_current_positions_;
   map_current_positions_ = new map<string, Vec>;

   string last_edge;
   string last_lane;
   string last_parsed_line = "";

   while ( last_parsed_round_ <= sc_->world_w().simulation_round() )
   {
      // read next line
      if ( !getline( ifs_dump_file_ , last_parsed_line ) )
         break;
      // look for time-step info
      if ( last_parsed_line.find( "<timestep" ) != string::npos )
      {
         unsigned int position = last_parsed_line.find( "time=" );
         if ( position != string::npos )
         {
            string str_round = "";
            unsigned int i = position + 6;
            while ( last_parsed_line[i] != '"' )
            {
               str_round += last_parsed_line[i];
               i++;
            }
            last_parsed_round_ = atol( str_round.c_str() );
         }
      }
      if ( last_parsed_round_ == sc_->world_w().simulation_round() ) // skip all rounds < sim round (first call)
      {
         if ( last_parsed_line.find( "<edge" ) != string::npos )
         {
            unsigned long position = last_parsed_line.find( "id=" );
            if ( position != string::npos )
            {
               last_edge.clear();
               unsigned long i = position + 4;
               while ( last_parsed_line[i] != '"' )
               {
                  last_edge += last_parsed_line[i];
                  i++;
               }
            }
         }
         // look for lane info
         else if ( last_parsed_line.find( "<lane" ) != string::npos )
         {
            unsigned long position = last_parsed_line.find( "id=" );
            if ( position != string::npos )
            {
               last_lane.clear();
               unsigned long i = position + 4;
               while ( last_parsed_line[i] != '"' )
               {
                  last_lane += last_parsed_line[i];
                  i++;
               }
            }
         }
         // look for vehicle
         else if ( last_parsed_line.find( "<vehicle" ) != string::npos )
         {
            SumoNet::SumoPosition  vehicle_position;
            vehicle_position.lane_position = 0.0;
            string str_id = "";

            // look for vehicle id (trip id)
            unsigned long position = last_parsed_line.find( "id=" );
            if ( position != string::npos )
            {
               str_id.clear();
               unsigned long i = position + 4;
               while ( last_parsed_line[i] != '"' )
               {
                  str_id += last_parsed_line[i];
                  i++;
               }
            }
            // look for position on lane
            position = last_parsed_line.find( "pos=" );
            if ( position != string::npos )
            {
               string str_pos = "";
               unsigned long i = position + 5;
               while ( last_parsed_line[i] != '"' )
               {
                  str_pos += last_parsed_line[i];
                  i++;
               }
               vehicle_position.lane_position = atof( str_pos.c_str() );
            }
            vehicle_position.edge_id = last_edge;
            vehicle_position.lane_id = last_lane;

            Vec vec_position = net_.sumo_pos_to_vec( vehicle_position );
            map_current_positions_->insert( make_pair( str_id, vec_position ) );
         } // end look for vehicle
      } // end if ( parsed round == simulation round )
   }
}

// -----------------------------------------------------------------------------

void SumoNodeMovementCreator::generate_movements()
{
   for ( map<string, Vec>::iterator iter = map_current_positions_->begin();
         iter != map_current_positions_->end();
         iter ++ )
   {
      map<string, Vec>::iterator iter_last = map_last_positions_->find( iter->first );

      MovementInfo * movement_info = new MovementInfo;
      Node * n = sc_->world_w().find_node_by_label_w( iter->first );
      movement_info->set_urgency( MovementInfo::Delayed );
      movement_info->set_dispatch_time( static_cast<double>( sc_->world_w().simulation_round() ) );
      movement_info->set_node( n );

      if ( iter_last != map_last_positions_->end() ) // existing node -> linear movement
      {
         LinearMovement * linear_movement = new LinearMovement;
         Vec speed = iter->second - iter_last->second;
         linear_movement->set_parameters( speed.euclidean_norm(), iter->second + speed, sc_->world_w() );
         movement_info->set_node_movement( linear_movement );
      }
      else // new node -> jump movement
      {
         JumpMovement * jump_movement = new JumpMovement;
         jump_movement->set_position( iter->second );
         movement_info->set_node_movement( jump_movement );
      }

      remaining_movements_.push_back( movement_info );
   }
}

// -----------------------------------------------------------------------------

void SumoNodeMovementCreator::add_new_nodes()
{
   for ( map<string, Vec>::iterator iter = map_current_positions_->begin();
         iter != map_current_positions_->end();
         iter ++ )
   {
      if ( map_last_positions_->find( iter->first ) == map_last_positions_->end() )
      {
         Node * n = new Node;
         n->set_label( iter->first );                   // label of nodes is trip id
         n->set_real_position( iter->second );

         for ( vector<ProcessorFactoryHandle>::const_iterator iter2 = processor_factories_.begin();
               iter2 != processor_factories_.end();
               iter2++ )
         {
            ProcessorHandle ph = iter2->get()->create();
            n->add_processor( ph );
         }

         sc_->world_w().add_node( *n );
      }

   }
}

// -----------------------------------------------------------------------------

void SumoNodeMovementCreator::delete_nodes()
{
   for ( map<string, Vec>::iterator iter = map_last_positions_->begin();
         iter != map_last_positions_->end();
         iter ++ )
   {
      if ( map_current_positions_->find( iter->first ) == map_current_positions_->end() )
      {
         sc_->world_w().transmission_model_w().deliver_messages();
         Node * n = sc_->world_w().find_node_by_label_w( iter->first );
         if ( n )
         {
            sc_->world_w().edge_model_w().node_removed( *n );
            sc_->world_w().remove_node( *n );
         }
      }
   }
}

// -----------------------------------------------------------------------------

void SumoNodeMovementCreator::create_dummy_node()
{
   shawn::Node * n = new shawn::Node;
   n->set_label( "Dummy node" );
   n->set_real_position( shawn::Vec( 0, 0 ) );

   for ( vector<ProcessorFactoryHandle>::const_iterator iter2 = processor_factories_.begin();
         iter2 != processor_factories_.end();
         iter2++ )
   {
      ProcessorHandle ph = iter2->get()->create();
      n->add_processor( ph );
   }

   sc_->world_w().add_node( *n );
}

}

#endif /* ENABLE_SUMO */

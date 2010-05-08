/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/ws_handler/node_control.h"
#include "apps/testbedservice/util/ws_helpers.h"
#include "apps/testbedservice/core/shawn_serverH.h"
#include "sys/processors/processor_keeper.h"
#include "sys/worlds/processor_world_factory.h"
#include "sys/misc/tokenize.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"
#include "sys/edge_model.h"
#include "sys/processor.h"
#include <boost/bind.hpp>
#include <sstream>

namespace testbedservice
{
   static testbedservice::NodeControl *node_control_ = 0;
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   NodeControl::
   NodeControl()
      : controller_( 0 ),
         sc_       ( 0 )
   {}
   // ----------------------------------------------------------------------
   NodeControl::
   ~NodeControl()
   {}
   // ----------------------------------------------------------------------
   void
   NodeControl::
   init( shawn::SimulationController& sc, TestbedServiceClient& controller )
      throw()
   {
      sc_ = &sc;
      controller_ = &controller;
      node_control_ = this;
   }
   // ----------------------------------------------------------------------
   void
   NodeControl::
   are_nodes_alive( std::string id, std::vector<std::string> nodes )
   {
      NodeIdVector response_nodes;
      StatusValueVector response_values;
      StatusMsgVector response_msgs;

      // TODO: Make this threadsafe with respect to Shawn; -> add to EventScheduler?
      for ( std::vector<std::string>::iterator
               it = nodes.begin();
               it != nodes.end();
               ++it )
      {
         const shawn::Node *node = simulation_controller().world().find_node_by_label( *it );
         if ( node )
         {
            if ( node->state() == shawn::Processor::Inactive )
               response_values.push_back( 0 );
            else
               response_values.push_back( 1 );
         }
         else
            response_values.push_back( -1 );

         response_nodes.push_back( *it );
         response_msgs.push_back( "" );
      }

      controller().send_receive_status( id, response_nodes, response_values, response_msgs );
   }
   // ----------------------------------------------------------------------
   void
   NodeControl::
   reset_nodes( std::string id, std::vector<std::string> nodes )
   {
      NodeIdVector response_nodes;
      StatusValueVector response_values;
      StatusMsgVector response_msgs;

      // TODO: Make this threadsafe with respect to Shawn; -> add to EventScheduler?
      for ( std::vector<std::string>::iterator
               it = nodes.begin();
               it != nodes.end();
               ++it )
      {
         const shawn::Node *node = simulation_controller().world().find_node_by_label( *it );
         if ( node )
         {
            // TODO: Add reset behavior in shawn::sys
            response_values.push_back( 0 );
         }
         else
            response_values.push_back( -1 );

         response_nodes.push_back( *it );
         response_msgs.push_back( "" );
      }

      controller().send_receive_status( id, response_nodes, response_values, response_msgs );
   }
   // ----------------------------------------------------------------------
   void
   NodeControl::
   set_node_state( std::string id, std::string urn, shawn::Processor::ProcessorState state )
   {
      NodeIdVector response_nodes;
      StatusValueVector response_values;
      StatusMsgVector response_msgs;

      response_nodes.push_back( urn );
      response_msgs.push_back( "" );
      shawn::Node *node = simulation_controller_w().world_w().find_node_by_label_w( urn );
      if ( node )
      {
         for ( shawn::Node::ProcessorList::iterator
                  it = node->begin_processors_w();
                  it != node->begin_processors_w();
                  ++it )
         {
            (**it).set_state( state );
         }
         response_values.push_back( 1 );
      }
      else
         response_values.push_back( -1 );

      controller().send_receive_status( id, response_nodes, response_values, response_msgs );
   }
   // ----------------------------------------------------------------------
   void
   NodeControl::
   pass_text_message( std::string id, NodeIdVector nodes, TextMessage message )
   {
      NodeIdVector response_nodes;
      StatusValueVector response_values;
      StatusMsgVector response_msgs;

      std::cout << "text message:" << std::endl;
      std::cout << "  -> from: " << message.source << std::endl;
      std::cout << "  -> msg : " << message.message << std::endl;

      for ( NodeIdVector::iterator it = nodes.begin(); it != nodes.end(); ++it )
      {
         shawn::Node *node =
            simulation_controller_w().world_w().find_node_by_label_w( *it );

         if ( node )
         {
            TestbedServiceProcessor *proc =
               node->get_processor_of_type_w<TestbedServiceProcessor>();
            if ( proc )
            {
               proc->process_text_message( message );
               response_values.push_back( 1 );
            }
            else
               response_values.push_back( 0 );
         }
         else
            response_values.push_back( -1 );

         response_nodes.push_back( *it );
         response_msgs.push_back( "" );
      }

      controller().send_receive_status( id, response_nodes, response_values, response_msgs );
   }
   // ----------------------------------------------------------------------
   void
   NodeControl::
   pass_binary_message( std::string id, NodeIdVector nodes, BinaryMessage message )
   {
      NodeIdVector response_nodes;
      StatusValueVector response_values;
      StatusMsgVector response_msgs;

      for ( NodeIdVector::iterator it = nodes.begin(); it != nodes.end(); ++it )
      {
         shawn::Node *node =
            simulation_controller_w().world_w().find_node_by_label_w( *it );

         if ( node )
         {
            TestbedServiceProcessor *proc =
               node->get_processor_of_type_w<TestbedServiceProcessor>();

            if ( proc )
            {
               proc->process_binary_message( message );
               response_values.push_back( 1 );
            }
            else
               response_values.push_back( 0 );
         }
         else
            response_values.push_back( -1 );

         response_nodes.push_back( *it );
         response_msgs.push_back( "" );
      }

      controller().send_receive_status( id, response_nodes, response_values, response_msgs );
   }
   // ----------------------------------------------------------------------
   NodeControl::NodeIdVector
   NodeControl::get_neighborhood( std::string urn )
   {
      NodeIdVector neighbors;
      const shawn::Node *node = simulation_controller().world().find_node_by_label( urn );
      if ( node )
      {
         for ( shawn::EdgeModel::const_adjacency_iterator
                  it = simulation_controller().world().begin_adjacent_nodes( *node );
                  it != simulation_controller().world().end_adjacent_nodes( *node );
                  ++it )
         {
            neighbors.push_back( it->label() );
         }
      }

      return neighbors;
   }
   // ----------------------------------------------------------------------
   void
   NodeControl::
   dbg_nodevector( std::string id, std::vector<std::string> nodes )
   {
      std::cout << "node vector:" << std::endl;
      std::cout << "  -> repsonse id: " << id << std::endl;
      std::cout << "  -> node cnt   : " << nodes.size() << std::endl;
      for ( std::vector<std::string>::iterator
               it = nodes.begin();
               it != nodes.end();
               ++it )
      {
         std::cout << "    **: " << *it << std::endl;
      }
   }

}

namespace shawn_server
{

   // -----------------------------------------------------------------------
   // -----------------------------------------------------------------------
   // -----------------------------------------------------------------------
   static bool node_control_available( void )
   {
      if ( testbedservice::node_control_ )
      {
         return true;
      }
      else
         std::cerr << "Error processing ws call - node_control_ not set." << std::endl;

      return false;
   }
   // -----------------------------------------------------------------------
   // ------------- Web Service Calls Network Control -----------------------
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __shawnts__areNodesAlive( soap*,
            shawnts__areNodesAlive *shawnts__areNodesAlive_,
            shawnts__areNodesAliveResponse *shawnts__areNodesAliveResponse_ )
   {
      if ( !node_control_available() )
         return SOAP_OK;

      int node_cnt = shawnts__areNodesAlive_->__sizenodes;
      std::vector<std::string> nodes;
      for ( int i = 0; i < node_cnt; i++ )
      {
         nodes.push_back( std::string(shawnts__areNodesAlive_->nodes[i]) );
      }

      std::string id = testbedservice::create_response_id();
      char *response_id = testbedservice::allocate_string( shawnts__areNodesAliveResponse_->soap, id );
      shawnts__areNodesAliveResponse_->return_ = response_id;

      boost::thread thrd( boost::bind(
         &testbedservice::NodeControl::are_nodes_alive,
         testbedservice::node_control_,
         id, nodes ) );

      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __shawnts__enableNode( soap*,
            shawnts__enableNode *shawnts__enableNode_,
            shawnts__enableNodeResponse *shawnts__enableNodeResponse_ )
   {
      std::string node = std::string( shawnts__enableNode_->node );
      std::string id = testbedservice::create_response_id();
      char *response_id = testbedservice::allocate_string( shawnts__enableNodeResponse_->soap, id );
      shawnts__enableNodeResponse_->return_ = response_id;

      boost::thread thrd( boost::bind(
         &testbedservice::NodeControl::set_node_state,
         testbedservice::node_control_,
         id, node, shawn::Processor::Active ) );

      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __shawnts__disableNode( soap*,
            shawnts__disableNode *shawnts__disableNode_,
            shawnts__disableNodeResponse *shawnts__disableNodeResponse_ )
   {
      std::string node = std::string( shawnts__disableNode_->node );
      std::string id = testbedservice::create_response_id();
      char *response_id = testbedservice::allocate_string( shawnts__disableNodeResponse_->soap, id );
      shawnts__disableNodeResponse_->return_ = response_id;

      boost::thread thrd( boost::bind(
         &testbedservice::NodeControl::set_node_state,
         testbedservice::node_control_,
         id, node, shawn::Processor::Inactive ) );

      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __shawnts__getNeighbourhood( soap*,
            shawnts__getNeighbourhood *shawnts__getNeighbourhood_,
            shawnts__getNeighbourhoodResponse *shawnts__getNeighbourhoodResponse_ )
   {
      if ( !node_control_available() )
         return SOAP_OK;

      std::string node( shawnts__getNeighbourhood_->node );
      std::vector<std::string> neighbors =
         testbedservice::node_control_->get_neighborhood( node );

      char **return_neighborhood_ = 0;
      if ( neighbors.size() > 0 )
      {
         struct soap *soap = shawnts__getNeighbourhoodResponse_->soap;
         return_neighborhood_ = (char**)soap_malloc( soap, neighbors.size() );
         int idx = 0;
         for ( std::vector<std::string>::iterator
                  it = neighbors.begin();
                  it != neighbors.end();
                  ++it )
         {
            return_neighborhood_[idx] = testbedservice::allocate_string( soap, *it );
            idx++;
         }
      }

      shawnts__getNeighbourhoodResponse_->__sizereturn_ = neighbors.size();
      shawnts__getNeighbourhoodResponse_->return_ = return_neighborhood_;

      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __shawnts__resetNodes( soap*,
            shawnts__resetNodes *shawnts__resetNodes_,
            shawnts__resetNodesResponse *shawnts__resetNodesResponse_ )
   {
      if ( !node_control_available() )
         return SOAP_OK;

      int node_cnt = shawnts__resetNodes_->__sizenodes;
      std::vector<std::string> nodes;
      for ( int i = 0; i < node_cnt; i++ )
      {
         nodes.push_back( std::string(shawnts__resetNodes_->nodes[i]) );
      }

      std::string id = testbedservice::create_response_id();
      char *response_id = testbedservice::allocate_string( shawnts__resetNodesResponse_->soap, id );
      shawnts__resetNodesResponse_->return_ = response_id;

      boost::thread thrd( boost::bind(
         &testbedservice::NodeControl::reset_nodes,
         testbedservice::node_control_,
         id, nodes ) );

      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __shawnts__getPropertyValueOf( soap*,
            shawnts__getPropertyValueOf *shawnts__getPropertyValueOf_,
            shawnts__getPropertyValueOfResponse *shawnts__getPropertyValueOfResponse_ )
   {
      // TODO !
      std::cout << "shawn::testbedservice::getPropertyValueOf" << std::endl;
      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __shawnts__send( soap*,
            shawnts__send *shawnts__send_,
            shawnts__sendResponse *shawnts__sendResponse_)
   {
      if ( !node_control_available() )
         return SOAP_OK;

      int node_cnt = shawnts__send_->__sizenodeIds;
      std::vector<std::string> nodes;
      for ( int i = 0; i < node_cnt; i++ )
      {
         nodes.push_back( std::string(shawnts__send_->nodeIds[i]) );
      }

      std::string id = testbedservice::create_response_id();
      std::string source( shawnts__send_->message->sourceNodeId );
      time_t timestamp = shawnts__send_->message->timestamp;
      switch ( shawnts__send_->message->__union_message )
      {
         case 1:
         {
            testbedservice::NodeControl::TextMessage message;
            message.source = source;
            message.timestamp = timestamp;
            message.message =
               std::string( shawnts__send_->message->union_message.textMessage->msg );
            // TODO: directly integrate soap-enum into struct?
            int level = *shawnts__send_->message->union_message.textMessage->messageLevel;
            message.level = (testbedservice::NodeControl::MessageLevel)level;

            boost::thread thrd( boost::bind(
               &testbedservice::NodeControl::pass_text_message,
               testbedservice::node_control_,
               id, nodes, message ) );
            break;
         }
         case 2:
         {
            testbedservice::NodeControl::BinaryMessage message;
            message.source = source;
            message.timestamp = timestamp;
            message.size =
                              shawnts__send_->message->union_message.binaryMessage->binaryData.__size;
            message.buffer = new uint8_t[message.size];
            memcpy( message.buffer,
                    (uint8_t*)shawnts__send_->message->union_message.binaryMessage->binaryData.__ptr,
                    message.size );

            boost::thread thrd( boost::bind(
               &testbedservice::NodeControl::pass_binary_message,
               testbedservice::node_control_,
               id, nodes, message ) );
            break;
         }
         default:
            // TODO: Some error handling?
            return SOAP_OK;
      }

      char *response_id = testbedservice::allocate_string( shawnts__sendResponse_->soap, id );
      shawnts__sendResponse_->return_ = response_id;

      return SOAP_OK;
   }
}

#endif

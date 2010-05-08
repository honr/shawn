/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/core/testbedservice_client.h"
#include "apps/testbedservice/core/shawn_clientControllerPortBindingProxy.h"
#include "apps/testbedservice/core/shawn_client.nsmap"
#include "apps/testbedservice/util/ws_helpers.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/node.h"
#include "sys/world.h"
#include <boost/bind.hpp>
#include <time.h>

namespace testbedservice
{

   TestbedServiceClient::
   TestbedServiceClient()
      : controller_uri_ ( "http://localhost:8080/controller" ),
         max_pool_size_ ( 5 )
   {}
   // --------------------------------------------------------------------
   TestbedServiceClient::
   ~TestbedServiceClient()
   {
      delete controller_;
   }
   // --------------------------------------------------------------------
   void
   TestbedServiceClient::
   init( const shawn::SimulationController& sc )
   {
      controller_ = new ControllerPortBindingProxy();

      controller_uri_ =
         sc.environment().optional_string_param( "controller_uri", controller_uri_ );
      controller().soap_endpoint = controller_uri_.c_str();

      max_pool_size_ = sc.environment().optional_int_param( "controller_max_parallel", max_pool_size_ );
      pool_.size_controller().resize( max_pool_size_ );
   }
   // --------------------------------------------------------------------
   std::string
   TestbedServiceClient::
   name( void )
      const throw()
   {
      return "testbedservice_client";
   }
   // --------------------------------------------------------------------
   std::string
   TestbedServiceClient::
   description( void )
      const throw()
   {
      return "Client that provides access (via webservice calls) to the Wisebed Controller";
   }
   // --------------------------------------------------------------------
   void
   TestbedServiceClient::
   send_receive_status( std::string id,
                        TestbedServiceClient::NodeIdVector& nodes,
                        TestbedServiceClient::StatusValueVector& values,
                        TestbedServiceClient::StatusMsgVector& msgs  )
      throw()
   {
      assert( nodes.size() == values.size() );
      assert( nodes.size() == msgs.size() );

      // prepare basic status types
      shawnts__receiveStatus *status = new shawnts__receiveStatus;
      shawnts__requestStatus *req = new shawnts__requestStatus;
      status->status = req;

      // set request id
      req->requestId = allocate_string( id );

      // fill status array
      int value_cnt = nodes.size();
      req->__sizestatus = value_cnt;
      req->status = new shawnts__status*[value_cnt];
      for ( int i = 0; i < value_cnt; i++ )
      {
         shawnts__status *st = new shawnts__status;
         st->nodeId = allocate_string( nodes.at(i) );
         st->value = allocate_int( values.at(i) );
         st->msg = allocate_string( msgs.at(i) );

         req->status[i] = st;
      }
      boost::lock_guard<boost::mutex> pool_lock( pool_mutex_ );
      pool_.schedule( boost::bind( &TestbedServiceClient::call_receive_status, this, status ) );
   }
   // --------------------------------------------------------------------
   void
   TestbedServiceClient::
   send_text_message( std::string& source, std::string& msg, int level )
      throw()
   {
      // prepare basic message types
      shawnts__receive *ts_receive = new shawnts__receive;
      ns2__message *ts_message = new ns2__message;
      ns2__textMessage *ts_text = new ns2__textMessage;
      ts_receive->msg = ts_message;
      ts_message->__union_message = 1;
      ts_message->union_message.textMessage = ts_text;

      ts_message->timestamp = time(NULL);
      ts_message->sourceNodeId = allocate_string( source );

      ts_text->msg = allocate_string( msg );
      ts_text->messageLevel = new ns2__messageLevel( (ns2__messageLevel)level );

      boost::lock_guard<boost::mutex> pool_lock( pool_mutex_ );
      pool_.schedule( boost::bind( &TestbedServiceClient::call_receive_message, this, ts_receive ) );
   }
   // --------------------------------------------------------------------
   void
   TestbedServiceClient::
   send_binary_message( std::string& source, int len, uint8_t* buf )
      throw()
   {
      // prepare basic message types
      shawnts__receive *ts_receive = new shawnts__receive;
      ns2__message *ts_message = new ns2__message;
      ns2__binaryMessage *ts_bin = new ns2__binaryMessage;
      ts_message->__union_message = 2;
      ts_message->union_message.binaryMessage = ts_bin;
      ts_receive->msg = ts_message;

      ts_message->timestamp = time(NULL);
      ts_message->sourceNodeId = allocate_string( source );

      uint8_t *buf_copy = new uint8_t( len );
      memcpy( buf_copy, buf, len );
      ts_bin->binaryData.__ptr = buf_copy;
      ts_bin->binaryData.__size = len;

      boost::lock_guard<boost::mutex> pool_lock( pool_mutex_ );
      pool_.schedule( boost::bind( &TestbedServiceClient::call_receive_message, this, ts_receive ) );
   }
   // --------------------------------------------------------------------
   bool
   TestbedServiceClient::
   send_test_data( void )
   {
      // text message
      std::string source = "my_first_node";
      std::string msg = "Huhu from Shawn!!";
      int level = 0;
      send_text_message( source, msg, level );

      // binary message
      std::string source2 = "my_second_node";
      uint8_t data[] = { 0xaf, 0xfe };
      send_binary_message( source2, sizeof(data), data );

      // status message
      NodeIdVector nodes;
      StatusValueVector values;
      StatusMsgVector msgs;

      nodes.push_back( "node-01" ); nodes.push_back( "node-02" );
      values.push_back(1); values.push_back(-1);
      msgs.push_back( "msg-01" ); msgs.push_back( "msg-02" );

      std::string id = std::string( create_response_id() );
      send_receive_status( id, nodes, values, msgs );
      id = std::string( create_response_id() );
      send_receive_status( id, nodes, values, msgs );
      id = std::string( create_response_id() );
      send_receive_status( id, nodes, values, msgs );
   }
   // --------------------------------------------------------------------
   void
   TestbedServiceClient::
   call_receive_status( shawnts__receiveStatus *rcv_status )
   {
      boost::lock_guard<boost::mutex> send_lock( send_mutex_ );
      if ( controller().receiveStatus( rcv_status ) == SOAP_OK )
      {
         std::cerr << "Send status ok" << std::endl;
      }
      else
      {
         std::cerr << "Send to Controller failed" << std::endl;
         if ( controller().soap_fault_string() )
            std::cerr << "  Reason: " << controller().soap_fault_string() << std::endl;
         if ( controller().soap_fault_detail() )
            std::cerr << "  Detail: " << controller().soap_fault_detail() << std::endl;
      }

      // at least, clean up...
      for ( int i = 0; i < rcv_status->status->__sizestatus; i++ )
      {
         delete rcv_status->status->status[i]->nodeId;
         delete rcv_status->status->status[i]->value;
         delete rcv_status->status->status[i]->msg;
         delete rcv_status->status->status[i];
      }
      delete[] rcv_status->status->status;
      delete rcv_status->status->requestId;
      delete rcv_status->status;
      delete rcv_status;
   }
   // --------------------------------------------------------------------
   void
   TestbedServiceClient::
   call_receive_message( shawnts__receive *receive )
   {
      boost::lock_guard<boost::mutex> send_lock( send_mutex_ );

      if ( controller().receive( receive ) == SOAP_OK )
      {
         std::cerr << "Send status ok" << std::endl;
      }
      else
      {
         std::cerr << "Send to Controller failed" << std::endl;
         if ( controller().soap_fault_string() )
            std::cerr << "  Reason: " << controller().soap_fault_string() << std::endl;
         if ( controller().soap_fault_detail() )
            std::cerr << "  Detail: " << controller().soap_fault_detail() << std::endl;
      }

      // at least, clean up...
      if ( receive->msg->__union_message == 1 )
      {
         delete receive->msg->union_message.textMessage->msg;
         delete receive->msg->union_message.textMessage->messageLevel;
         delete receive->msg->union_message.textMessage;
      }
      if ( receive->msg->__union_message == 2 )
      {
         delete receive->msg->union_message.binaryMessage->binaryData.__ptr;
         delete receive->msg->union_message.binaryMessage->binaryData.id;
         delete receive->msg->union_message.binaryMessage->binaryData.type;
         delete receive->msg->union_message.binaryMessage->binaryData.options;
         delete receive->msg->union_message.binaryMessage->binaryType;
         delete receive->msg->union_message.binaryMessage;
      }
      delete receive->msg->sourceNodeId;
      delete receive->msg;
      delete receive;
   }

}

#endif

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/core/wsnapi_client.h"
#include "apps/testbedservice/util/ws_helpers.h"
#include "apps/testbedservice/core/wsnapi_clientWSNPortBindingProxy.h"
#include "apps/testbedservice/core/wsnapi_client.nsmap"
#include <boost/bind.hpp>
#include <time.h>

using namespace wsnapi_client;

namespace testbedservice
{

   WsnApiClient::
   WsnApiClient()
      : wsnapi_uri_ ( "http://localhost:8080/controller" ),
         max_pool_size_ ( 5 ),
         wsnapi_        ( 0 )
   {}
   // --------------------------------------------------------------------
   WsnApiClient::
   ~WsnApiClient()
   {
      delete wsnapi_;
   }
   // --------------------------------------------------------------------
   void
   WsnApiClient::
   init( std::string uri )
   {
      wsnapi_ = new WSNPortBindingProxy();

      if ( !uri.empty() )
      {
         wsnapi_uri_ = uri;
         wsnapi().soap_endpoint = wsnapi_uri_.c_str();
      }

      pool_.size_controller().resize( max_pool_size_ );
   }
   // --------------------------------------------------------------------
   void
   WsnApiClient::
   send_text_message( NodeIdVector dests, TextMessage& text, bool threaded )
      throw()
   {
      // prepare basic message types
      shawnts__send *send = new shawnts__send;
      ns2__message *message = new ns2__message;
      ns2__textMessage *text_message = new ns2__textMessage;

      send->message = message;
      send->__sizenodeIds = dests.size();
      send->nodeIds = new char*[ dests.size() ];
      for ( unsigned int i = 0; i < dests.size(); i++ )
      {
         send->nodeIds[i] = allocate_string( dests.at(i) );;
      }

      message->__union_message = 1;
      message->union_message.textMessage = text_message;
      message->timestamp = text.timestamp;
      message->sourceNodeId = allocate_string( text.source );
      text_message->msg = allocate_string( text.message );
      text_message->messageLevel = new ns2__messageLevel( (ns2__messageLevel)text.level );

      if ( threaded )
      {
         boost::lock_guard<boost::mutex> pool_lock( pool_mutex_ );
         pool_.schedule( boost::bind( &WsnApiClient::call_send_message, this, send ) );
      }
      else
         call_send_message( send );
   }
   // --------------------------------------------------------------------
   void
   WsnApiClient::
   send_binary_message( NodeIdVector dests, BinaryMessage& bin, bool threaded )
      throw()
   {
      // prepare basic message types
      shawnts__send *send = new shawnts__send;
      ns2__message *message = new ns2__message;
      ns2__binaryMessage *binary_message = new ns2__binaryMessage;

      send->message = message;
      send->__sizenodeIds = dests.size();
      send->nodeIds = new char*[ dests.size() ];
      for ( unsigned int i = 0; i < dests.size(); i++ )
      {
         send->nodeIds[i] = allocate_string( dests.at(i) );;
      }

      message->timestamp = bin.timestamp;
      message->sourceNodeId = allocate_string( bin.source );
      message->__union_message = 2;
      message->union_message.binaryMessage = binary_message;

      uint8_t *buf_copy = new uint8_t[bin.size];
      memcpy( buf_copy, bin.buffer, bin.size );
      binary_message->binaryData.__ptr = buf_copy;
      binary_message->binaryData.__size = bin.size;

      if ( threaded )
      {
         boost::lock_guard<boost::mutex> pool_lock( pool_mutex_ );
         pool_.schedule( boost::bind( &WsnApiClient::call_send_message, this, send ) );
      }
      else
         call_send_message( send );
   }
   // --------------------------------------------------------------------
   void
   WsnApiClient::
   call_send_message( shawnts__send *send )
   {
      boost::lock_guard<boost::mutex> send_lock( send_mutex_ );

      shawnts__sendResponse *shawnts__sendResponse = 0;
      if ( wsnapi().send( send, shawnts__sendResponse ) == SOAP_OK )
      {
         std::cerr << "Send status ok" << std::endl;
      }
      else
      {
         std::cerr << "Send to WSN API failed" << std::endl;
         if ( wsnapi().soap_fault_string() )
            std::cerr << "  Reason: " << wsnapi().soap_fault_string() << std::endl;
         if ( wsnapi().soap_fault_detail() )
            std::cerr << "  Detail: " << wsnapi().soap_fault_detail() << std::endl;
      }

      // at least, clean up...
      if ( send->message->__union_message == 1 )
      {
         delete send->message->union_message.textMessage->msg;
         delete send->message->union_message.textMessage->messageLevel;
         delete send->message->union_message.textMessage;
      }
      if ( send->message->__union_message == 2 )
      {
         delete send->message->union_message.binaryMessage->binaryData.__ptr;
         delete send->message->union_message.binaryMessage->binaryData.id;
         delete send->message->union_message.binaryMessage->binaryData.type;
         delete send->message->union_message.binaryMessage->binaryData.options;
         delete send->message->union_message.binaryMessage->binaryType;
         delete send->message->union_message.binaryMessage;
      }
      delete send->message->sourceNodeId;
      delete send->message;
      delete send;
   }

}

#endif
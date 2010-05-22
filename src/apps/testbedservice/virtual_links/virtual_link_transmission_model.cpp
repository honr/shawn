/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/virtual_links/virtual_link_transmission_model.h"
#include "apps/testbedservice/virtual_links/virtual_link_message.h"
#include "apps/testbedservice/util/types.h"
#include "legacyapps/wiselib/ext_iface_processor.h"
#include "sys/edge_model.h"
#include "sys/world.h"
#include <algorithm>
#include <sstream>


namespace testbedservice
{

   // ----------------------------------------------------------------------
   VirtualLinkTransmissionModel::
   VirtualLinkTransmissionModel()
      : client_ ( 0 )
   {}
   // ----------------------------------------------------------------------
   VirtualLinkTransmissionModel::
   ~VirtualLinkTransmissionModel()
   {}
   // ----------------------------------------------------------------------
   void
   VirtualLinkTransmissionModel::
   init( void )
      throw()
   {
      ChainableTransmissionModel::init();
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkTransmissionModel::
   reset( void )
      throw()
   {
      ChainableTransmissionModel::reset();
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkTransmissionModel::
   send_message( shawn::TransmissionModel::MessageInfo& mi )
      throw()
   {
      WiselibMessage *message =
         dynamic_cast<WiselibMessage*>( mi.msg_.get() );
      if ( message )
      {
         for ( VirtualLinkListIterator
               it = virtual_links_.begin();
               it != virtual_links_.end();
               ++it )
            if ( (*it)->shawn_node == message->source().id() )
            {
               // TODO: Instead of just "breaking", it should be possible to
               //       have multiple "clients" per shawn node - so that one
               //       message could be passed to multiple portal servers.
               pass_to_webservice_client( *message, *(*it)->wsnapi_client );
               break;
            }
      }

      pass_to_chain( mi );
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkTransmissionModel::
   pass_to_webservice_client( WiselibMessage& message, WsnApiClient& client )
      throw()
   {
      VirtualLinkMessage vmsg;
      vmsg.type = NODE_OUTPUT_VIRTUAL_LINK;
      vmsg.rssi = 127;
      vmsg.lqi = 127;
      vmsg.payload_size = message.payload_size();
      vmsg.destination = message.destination();
      vmsg.source = message.source().id();
      vmsg.payload = new uint8_t[vmsg.payload_size];
      memcpy( vmsg.payload, message.payload(), vmsg.payload_size );
      std::cout << "Send " << vmsg << std::endl;

      std::stringstream ssource;
      ssource << message.source().id();
      std::string source = ssource.str();

      std::stringstream sdest;
      sdest << "urn:wisebed:tubs:" << message.destination();
      std::string destination = sdest.str();
      NodeIdVector dests;
      dests.push_back( destination );

      uint8_t len = vmsg.length();
      uint8_t *bytes = vmsg.to_bytes();

      BinaryMessage bm;
      bm.source = source;
      bm.timestamp = time(0);
      bm.size = len;
      bm.buffer = bytes;

      testbedservice_client().send_binary_message( source, len, bytes );
      client.send_binary_message( dests, bm );
      delete bytes;
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkTransmissionModel::
   timeout( shawn::EventScheduler& ev, shawn::EventScheduler::EventHandle eh,
            double t, shawn::EventScheduler::EventTagHandle& eth )
      throw ()
   {
      VirtualLinkMessage *msg = dynamic_cast<VirtualLinkMessage*>( eth.get() );
      if ( msg )
      {
         std::cout << "Rcvd " << *msg << std::endl;

         for ( VirtualLinkListIterator
                  it = virtual_links_.begin();
                  it != virtual_links_.end();
                  ++it )
            if ( (*it)->virtual_node == msg->source )
            {
               std::cout << " -> deliver to: " << (*it)->shawn_node
                  << "; addressed to " << (int)msg->destination << std::endl;
               shawn::Node* node = world_w().find_node_by_id_w( (*it)->shawn_node );
               if ( node )
               {
                  wiselib::ExtIfaceProcessor *extiface =
                     node->get_processor_of_type_w<wiselib::ExtIfaceProcessor>();
                  if ( extiface )
                  {
                     extiface->receive_vlink( msg );
                     std::cout << "*** ok, delivered " << std::endl;
                  }
               }
            }
      }
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkTransmissionModel::
   add_virtual_link( int shawn_node, int virtual_node, std::string remote_uri )
      throw()
   {
      if ( find_virtual_link_w( shawn_node, virtual_node ) != virtual_links_.end() )
         return;

      VirtualLink *link = new VirtualLink();
      link->shawn_node = shawn_node;
      link->virtual_node = virtual_node;
      link->remote_uri = remote_uri;

      if ( wsnapis_clients_.find( remote_uri ) == wsnapis_clients_.end() )
      {
         std::cout << "create new wsn api to " << remote_uri << std::endl;
         WsnApiClient *client = new WsnApiClient();
         client->init( remote_uri );
         wsnapis_clients_[remote_uri] = client;
         link->wsnapi_client = client;
      }
      else
      {
         std::cout << "use existing wsn api to " << remote_uri << std::endl;
         link->wsnapi_client = wsnapis_clients_[remote_uri];
      }

      virtual_links_.push_back( link );
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkTransmissionModel::
   remove_virtual_link( int shawn_node, int virtual_node )
      throw()
   {
      VirtualLinkListIterator it =
         find_virtual_link_w( shawn_node, virtual_node );

      if ( it == virtual_links_.end() )
         return;

      VirtualLink *link = *it;
      virtual_links_.erase( it );
      delete link;
   }
   // ----------------------------------------------------------------------
   VirtualLinkTransmissionModel::VirtualLinkListIterator
   VirtualLinkTransmissionModel::
   find_virtual_link_w( int shawn_node, int virtual_node )
      throw()
   {
      for ( VirtualLinkListIterator
               it = virtual_links_.begin();
               it != virtual_links_.end();
               ++it )
      {
         if ( (*it)->shawn_node == shawn_node &&
                (*it)->virtual_node == virtual_node )
            return it;
      }

      return virtual_links_.end();
   }

}

#endif

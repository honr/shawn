/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/ws_handler/virtual_link_transmission_model.h"
#include "legacyapps/wiselib/ext_iface_processor.h"
#include "sys/edge_model.h"
#include "sys/world.h"
#include <algorithm>

namespace testbedservice
{

   // ----------------------------------------------------------------------
   VirtualLinkTransmissionModel::
   VirtualLinkTransmissionModel()
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
// //       socket_client_.start_client( world().simulation_controller() );
// // 
// //       testbedservice::TestbedServiceHandlerKeeper *whk =
// //          world_w().simulation_controller_w().
// //          keeper_by_name_w<testbedservice::TestbedServiceHandlerKeeper>
// //                                           ("TestbedServiceHandlerKeeper");
// // 
// //       if ( whk )
// //       {
// //          vlink_ = dynamic_cast<VirtualLinkControl*>
// //                                  ( whk->find_w( "virtual_link" ).get() );
// //          if ( vlink_ )
// //          {
// //             vlink_->add_handler( this );
// //          }
// //          else
// //          {
// //             std::cerr << "TestbedServiceVirtualLink is of wrong type." << std::endl;
// //             abort();
// //          }
// //       }
// //       else
// //       {
// //          std::cerr << "TestbedServiceHandlerKeeper not registered." << std::endl;
// //          abort();
// //       }
// // 
// //       std::cout << "TestbedService::virtual_link_transm: Initialised." << std::endl;
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
               pass_to_webservice_client( *message );
               break;
            }
      }

      pass_to_chain( mi );
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkTransmissionModel::
   pass_to_webservice_client( WiselibMessage& message )
      throw()
   {
      VirtualLinkMessage vmsg;
      vmsg.destination = message.destination();
      vmsg.source = message.source().id();
      vmsg.rssi = 127;
      vmsg.lqi = 127;
      vmsg.payload_size = message.payload_size();
      vmsg.payload = new uint8_t[vmsg.payload_size];
      memcpy( vmsg.payload, message.payload(), vmsg.payload_size );

      std::cout << "Send " << vmsg << std::endl;

      uint8_t *bytes = vmsg.to_bytes();
//       bool ret = socket_client_.send_data( bytes, vmsg.length() );
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
// //                      extiface->receive_vlink( msg );
#warning whats up with wiselib proc???
                     std::cout << "*** ok, delivered " << std::endl;
                  }
               }
            }

         std::cout << "AND ECHO...." <<      std::endl;
         uint64_t temp = msg->source;
         msg->source = msg->destination;
         msg->destination = temp;

//          socket_client_.send_data( (uint8_t*)msg, msg->length() );
      }
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkTransmissionModel::
   add_virtual_link( int shawn_node, int virtual_node )
      throw()
   {
      if ( find_virtual_link_w( shawn_node, virtual_node ) !=
                                                      virtual_links_.end() )
         return;

      VirtualLink *link = new VirtualLink();
      link->shawn_node = shawn_node;
      link->virtual_node = virtual_node;

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

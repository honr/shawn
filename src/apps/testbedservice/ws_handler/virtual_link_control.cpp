/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/ws_handler/virtual_link_control.h"
#include "apps/testbedservice/virtual_links/virtual_link_transmission_model.h"
#include "apps/testbedservice/virtual_links/virtual_link_message.h"
#include "apps/testbedservice/core/shawn_serverH.h"
#include "sys/processors/processor_keeper.h"
#include "sys/worlds/processor_world_factory.h"
#include "sys/misc/random/basic_random.h"
#include "sys/logging/logger.h"
#include "sys/misc/tokenize.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"
#include "sys/processor.h"
#include <boost/bind.hpp>

namespace testbedservice
{

   static testbedservice::VirtualLinkControl *virtual_link_control_ = 0;
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   VirtualLinkControl::
   VirtualLinkControl()
      : vltm_        ( 0 ),
         controller_ ( 0 ),
         sc_         ( 0 )
   {}
   // ----------------------------------------------------------------------
   VirtualLinkControl::
   ~VirtualLinkControl()
   {}
   // ----------------------------------------------------------------------
   void
   VirtualLinkControl::
   timeout( shawn::EventScheduler&,
            shawn::EventScheduler::EventHandle,
            double,
            shawn::EventScheduler::EventTagHandle& eth )
      throw()
   {
      std::cout << "TIMEOUT AT " << sc_->world().current_time() << std::endl;

      VirtualLinkMessage *msg = dynamic_cast<VirtualLinkMessage*>( eth.get() );
      if ( msg )
      {
         std::cout << "Got VirtualLinkMessage from " << msg->source  << std::endl;
      }
      else
         std::cout << "ETH of unknown type" << std::endl;
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkControl::
   init( shawn::SimulationController& sc, TestbedServiceClient& controller, VirtualLinkTransmissionModel *vltm )
      throw()
   {
      sc_ = &sc;
      controller_ = &controller;
      vltm_ = vltm;
      virtual_link_control_ = this;
   }
   // ----------------------------------------------------------------------
   void
   VirtualLinkControl::
   add_virtual_message( std::string dest, BinaryMessage message )
      throw()
   {
      boost::lock_guard<boost::mutex> sc_lock( sc_mutex_ );

      if ( sc_ )
      {
         // TODO: check if buffer is of correct size for vlink message
         VirtualLinkMessage *vlink_msg = new VirtualLinkMessage( message.buffer );

         std::cout << "CALLED AT " << sc_->world().current_time() << std::endl;
         double now = sc_->world().current_time();
         sc_->world_w().scheduler_w().new_event( virtual_link_transmission_model_w(), now, vlink_msg );
//          sc_->world_w().scheduler_w().new_event( *this, now, vlink_msg );
         std::cout << "ADD EVENT REMOTELY AT " << now << std::endl;
      }
   }
}

namespace shawn_server
{

   static bool virtual_link_control_available( void )
   {
      if ( testbedservice::virtual_link_control_ )
      {
         return true;
      }
      else
         std::cerr << "Error processing ws call - virtual_link_control_ not set." << std::endl;

      return false;
   }

   // -----------------------------------------------------------------------
   // ------------- Web Service Calls Network Control -----------------------
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __shawnts__setVirtualLink( soap*,
            shawnts__setVirtualLink *shawnts__setVirtualLink_,
            shawnts__setVirtualLinkResponse *shawnts__setVirtualLinkResponse_ )
   {
      // TODO !
      std::cout << "shawn::testbedservice::setVirtualLink" << std::endl;
      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __shawnts__destroyVirtualLink( soap*,
            shawnts__destroyVirtualLink *shawnts__destroyVirtualLink_,
            shawnts__setVirtualLinkResponse *shawnts__destroyVirtualLinkResponse_)
   {
      // TODO !
      std::cout << "shawn::testbedservice::destroyVirtualLink" << std::endl;
      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __shawnts__enablePhysicalLink( soap*,
            shawnts__enablePhysicalLink *shawnts__enablePhysicalLink_,
            shawnts__enablePhysicalLinkResponse *shawnts__enablePhysicalLinkResponse_)
   {
      // TODO !
      std::cout << "shawn::testbedservice::enableVirtualLink" << std::endl;
      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __shawnts__disablePhysicalLink( soap*,
            shawnts__disablePhysicalLink *shawnts__disablePhysicalLink_,
            shawnts__disablePhysicalLinkResponse *shawnts__disablePhysicalLinkResponse_)
   {
      // TODO !
      std::cout << "shawn::testbedservice::disableVirtualLink" << std::endl;
      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
    SOAP_FMAC5 int SOAP_FMAC6 __shawnts__getFilters( soap*,
            shawnts__getFilters *shawnts__getFilters_,
            shawnts__getFiltersResponse *shawnts__getFiltersResponse_)
   {
      // TODO !
      std::cout << "shawn::testbedservice::getFilters" << std::endl;
      return SOAP_OK;
   }

}

#endif

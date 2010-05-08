/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE
#include "apps/testbedservice/processor/testbedservice_processor.h"
#include "apps/testbedservice/ws_handler/testbedservice_control_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/node.h"
#include "sys/world.h"


namespace testbedservice
{
   TestbedServiceProcessor::
   TestbedServiceProcessor()
      : controller_ ( 0 )
   {}
   // ----------------------------------------------------------------------
   TestbedServiceProcessor::
   ~TestbedServiceProcessor()
   {}
   // ----------------------------------------------------------------------
   void
   TestbedServiceProcessor::
   boot()
      throw()
   {
      TestbedserviceControlKeeper *tck =
         owner_w().world_w().simulation_controller_w().
         keeper_by_name_w<TestbedserviceControlKeeper>("TestbedserviceKeeper");

      if ( tck )
      {
         controller_ = dynamic_cast<TestbedServiceClient*>
                            ( tck->find_w( "testbedservice_client" ).get() );
         if ( !controller_ )
         {
            std::cerr << "'TestbedServiceClient' not found in Keeper." << std::endl;
            abort();
         }
      }
      else
      {
         std::cerr << "'TestbedserviceKeeper' not found." << std::endl;
         abort();
      }

      std::cerr << "'TestbedServiceClient' successfully loaded." << std::endl;
   }
   // ----------------------------------------------------------------------
   void
   TestbedServiceProcessor::
   process_text_message( TextMessage& message )
      throw()
   {
      // should be overwritten by derived processors - otherwise text message
      // is just ignored
   }
   // ----------------------------------------------------------------------
   void
   TestbedServiceProcessor::
   process_binary_message( BinaryMessage& message )
      throw()
   {
      // should be overwritten by derived processors - otherwise text message
      // is just ignored
   }
   // ----------------------------------------------------------------------
   void
   TestbedServiceProcessor::
   send_text_message( std::string& message, MessageLevel level )
      throw()
   {
      std::string source = owner().label();
      controller().send_text_message( source, message, int(level) );
   }
   // ----------------------------------------------------------------------
   void
   TestbedServiceProcessor::
   send_binary_message( int length, uint8_t *buffer )
      throw()
   {
      std::string source = owner().label();
      controller().send_binary_message( source, length, buffer );
   }
   // ----------------------------------------------------------------------
   void
   TestbedServiceProcessor::
   flash_program( FlashProgram& program )
      throw()
   {}

}
#endif

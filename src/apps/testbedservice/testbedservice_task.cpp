/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/testbedservice_task.h"
#include "apps/testbedservice/core/testbedservice_client.h"
#include "apps/testbedservice/ws_handler/testbedservice_control_keeper.h"
#include "sys/processors/processor_keeper.h"
#include "sys/worlds/processor_world_factory.h"
#include "sys/misc/random/basic_random.h"
#include "sys/logging/logger.h"
#include "sys/misc/tokenize.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"
#include "sys/processor.h"
#include <boost/thread.hpp>


namespace testbedservice
{

   TestbedServiceTask::
   TestbedServiceTask()
   {}
   // ----------------------------------------------------------------------
   TestbedServiceTask::
   ~TestbedServiceTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   TestbedServiceTask::
   name( void )
      const throw()
   {
      return "testbedservice";
   }
   // ----------------------------------------------------------------------
   std::string
   TestbedServiceTask::
   description( void )
      const throw()
   {
      return "Start testbedservice for remote access; connection to portal server";
   }
   // ----------------------------------------------------------------------
   void
   TestbedServiceTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      require_world( sc );

      TestbedserviceControlKeeper *tck =
         sc.world_w().simulation_controller_w().
         keeper_by_name_w<TestbedserviceControlKeeper>("TestbedserviceKeeper");

      TestbedServiceClient *client = new testbedservice::TestbedServiceClient();
      if ( tck )
      {
        tck->add( client );
      }
      else
      {
         std::cerr << "'TestbedserviceKeeper' not found." << std::endl;
         abort();
      }
      client->init(sc);

      // TODO: add these control instances also to control keeper
      experiment_control_.init( sc, *client );
      network_control_.init( sc, *client );
      node_control_.init( sc, *client );
      virtual_link_control_.init( sc, *client );

      testbedservice_server_.start_server( sc );


      // WAIT FOR SERVER TO STARTUP
      std::cout << "wait for server to startup (1/2 second)..." << std::endl;
      boost::this_thread::sleep( boost::posix_time::milliseconds( 500 ) );

      std::cout << "send test data to controller..." << std::endl;
      client->send_test_data();
      std::cout << "ok" << std::endl;
   }

}
#endif

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "routing_init.h"
#ifdef ENABLE_ROUTING

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"

#include "apps/routing/routing_init.h"
#include "apps/routing/routing_protocol_factory.h"
#include "apps/routing/routing_protocol_keeper.h"
#include "apps/routing/routing_protocol_task.h"

#include "apps/routing/test/routing_test.h"
#include "apps/routing/dummy/dummy_routing_factory.h"
#include "apps/routing/flooding/flood_routing_factory.h"
#include "apps/routing/flooding/flood_routing_processor_factory.h"

#include "apps/routing/geo/geo_routing_processor_factory.h"
#include "apps/routing/geo/geo_routing_factory.h"
#include "apps/routing/geo/geo_routing_protocol.h"
#include "apps/routing/geo/test/geo_test.h"

extern "C" void init_routing( shawn::SimulationController& sc )
{
    //Create two keepers in the simulation controller 
    //One for the factories and one for the created routing protocol instances
    sc.add_keeper( new routing::RoutingProtocolKeeper );
    sc.add_keeper( new routing::RoutingProtocolFactoryKeeper );
    routing::RoutingTestProcessor::register_factory(sc);
    sc.simulation_task_keeper_w().add( new routing::RoutingProtocolTask );

    //Obtain the factory (redundant to ensure the proper working of the method below
    routing::RoutingProtocolFactoryKeeper& rfk = routing::routing_protocol_factory_keeper_w(sc);

    //Dummy routing
    {
        rfk.add( new routing::dummy::DummyRoutingProtocolFactory );    
    }

    //Flooding
    {
        rfk.add( new routing::flood::FloodRoutingProtocolFactory );
        routing::flood::FloodRoutingProcessorFactory::register_factory(sc);
    }

	//Geo
	{
		rfk.add( new routing::geo::GeoRoutingProtocolFactory()  );

		routing::geo::GeoRoutingProcessorFactory::register_factory(sc);
		routing::geo::GeoTestProcessor::Factory::register_task(sc);
		routing::geo::GeoTestProcessor::Factory::register_factory(sc);
	}

}


#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/routing_init.cpp,v $
* Version $Revision: 1.3 $
* Date    $Date: 2007/01/16 15:55:11 $
*-----------------------------------------------------------------------
* $Log: routing_init.cpp,v $
 *-----------------------------------------------------------------------*/

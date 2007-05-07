/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "routing_protocol_task.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/routing_init.h"
#include "apps/routing/routing_protocol_factory.h"
#include "apps/routing/routing_protocol_keeper.h"
#include "apps/routing/routing_protocol_task.h"
#include "sys/simulation/simulation_controller.h"
#include <sstream>
#include <string>
using namespace std;

namespace routing 
{

    // ----------------------------------------------------------------------
    RoutingProtocolTask::
        RoutingProtocolTask()
    {}

    // ----------------------------------------------------------------------
    RoutingProtocolTask::
        ~RoutingProtocolTask()
    {}

    // ----------------------------------------------------------------------
    std::string
        RoutingProtocolTask::
        name( void )
        const throw()
    {
        return "create_routing_protocol";
    }
    // ----------------------------------------------------------------------
    std::string
        RoutingProtocolTask::
        description( void )
        const throw()
    {
        return "Creates a new routing protocol instance with $name of the type $protocol and "
            "places this instance in the routing protocol keeper. This keeper can be obtained "
            "using the method routing::routing_protocol_keeper() and routing::routing_protocol_keeper_w().";
    }

    // ----------------------------------------------------------------------
    void
        RoutingProtocolTask::
        run( shawn::SimulationController& sc )
        throw( std::runtime_error )
    {
		//Retrieve configuration settings (name of the protocol instance and protocol type)
        const shawn::SimulationEnvironment& env = sc.environment();
        string prot_name = env.required_string_param("protocol");
        string inst_name = env.required_string_param("name");

		//Get a reference to the routing protocol factory of the requested procotol
        routing::RoutingProtocolFactoryKeeper& rfk = routing::routing_protocol_factory_keeper_w(sc);
        RoutingProtocolFactoryHandle rpfh = rfk.find_w(prot_name);

		//Throw an exception if the factory has not been found
        if( rpfh.is_null() )
        {
            ostringstream oss;
            oss << "No factory for the protocol named '"<< prot_name <<"' found. Available factories are: " << endl;
            rfk.print_contents(oss, true);
            throw std::runtime_error(oss.str());
        }

		//Create the new routing protocol instance and put it into the keeper
        routing::RoutingProtocolKeeper& rk = routing::routing_protocol_keeper_w(sc);
        rk.add( rpfh->create(inst_name, sc) );
    }

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/routing_protocol_task.cpp,v $
* Version $Revision: 1.1 $
* Date    $Date: 2005/10/11 15:24:21 $
*-----------------------------------------------------------------------
* $Log: routing_protocol_task.cpp,v $
 *-----------------------------------------------------------------------*/

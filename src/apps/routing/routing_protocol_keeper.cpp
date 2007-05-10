/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "routing_protocol_keeper.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/routing_protocol_keeper.h"
#include "apps/routing/routing_protocol_factory.h"
#include "apps/routing/routing.h"
#include "sys/util/handle_keeper.h"
#include "sys/simulation/simulation_controller.h"

namespace routing {

    const std::string RoutingProtocolKeeper::KEEPER_NAME( "routing_protocol" );


    // ----------------------------------------------------------------------
    RoutingProtocolKeeper::
        RoutingProtocolKeeper()
        : shawn::HandleKeeper<RoutingProtocolBase>( KEEPER_NAME, "Routing protocols" )
    {}
    // ----------------------------------------------------------------------
    RoutingProtocolKeeper::
        ~RoutingProtocolKeeper()
    {}

    // ----------------------------------------------------------------------
    const RoutingProtocolKeeper& 
        routing_protocol_keeper(const shawn::SimulationController& sc)
        throw()
    {
        const RoutingProtocolKeeper* k = sc.keeper_by_name<RoutingProtocolKeeper>(RoutingProtocolKeeper::KEEPER_NAME);
        assert( k != NULL );
        return *k;

    }

    // ----------------------------------------------------------------------
    RoutingProtocolKeeper& 
        routing_protocol_keeper_w(shawn::SimulationController& sc) 
        throw()
    {
        RoutingProtocolKeeper* k = sc.keeper_by_name_w<RoutingProtocolKeeper>(RoutingProtocolKeeper::KEEPER_NAME);
        assert( k != NULL );
        return *k;
    }





    const std::string RoutingProtocolFactoryKeeper::KEEPER_NAME( "routing_protocol_factory" );

    // ----------------------------------------------------------------------
    RoutingProtocolFactoryKeeper::
        RoutingProtocolFactoryKeeper()
        : shawn::HandleKeeper<RoutingProtocolFactory>( KEEPER_NAME, "Routing protocol factories" )
    {}
    // ----------------------------------------------------------------------
    RoutingProtocolFactoryKeeper::
        ~RoutingProtocolFactoryKeeper()
    {}

    // ----------------------------------------------------------------------
    const RoutingProtocolFactoryKeeper& 
        routing_protocol_factory_keeper(const shawn::SimulationController& sc)
        throw()
    {
        const RoutingProtocolFactoryKeeper* k = sc.keeper_by_name<RoutingProtocolFactoryKeeper>(RoutingProtocolKeeper::KEEPER_NAME);
        assert( k != NULL );
        return *k;

    }

    // ----------------------------------------------------------------------
    RoutingProtocolFactoryKeeper& 
        routing_protocol_factory_keeper_w(shawn::SimulationController& sc) 
        throw()
    {
        RoutingProtocolFactoryKeeper* k = sc.keeper_by_name_w<RoutingProtocolFactoryKeeper>(RoutingProtocolFactoryKeeper::KEEPER_NAME);
        assert( k != NULL );
        return *k;
    }
}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/routing_protocol_keeper.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: routing_protocol_keeper.cpp,v $
 *-----------------------------------------------------------------------*/

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_ROUTING_PROTOCOL_KEEPER_H
#define __SHAWN_APPS_ROUTING_PROTOCOL_KEEPER_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/routing.h"
#include "apps/routing/routing_protocol_factory.h"
#include "sys/util/handle_keeper.h"

namespace shawn {
    class SimulationController;
}

namespace routing {

    //------------------------------------------------------------------------
    /**
      *
      */
    class RoutingProtocolKeeper
        : public shawn::HandleKeeper<RoutingProtocolBase>
    {
    public:
        static const std::string KEEPER_NAME;

        RoutingProtocolKeeper();
        virtual ~RoutingProtocolKeeper();
    };

    const RoutingProtocolKeeper& routing_protocol_keeper(const shawn::SimulationController&) throw();
    RoutingProtocolKeeper& routing_protocol_keeper_w(shawn::SimulationController&) throw();

    //------------------------------------------------------------------------
    /**
      *
      */
    class RoutingProtocolFactoryKeeper
        : public shawn::HandleKeeper<RoutingProtocolFactory>
    {
    public:
        static const std::string KEEPER_NAME;

        RoutingProtocolFactoryKeeper();
        virtual ~RoutingProtocolFactoryKeeper();
    };


    const RoutingProtocolFactoryKeeper& routing_protocol_factory_keeper(const shawn::SimulationController&) throw();
    RoutingProtocolFactoryKeeper& routing_protocol_factory_keeper_w(shawn::SimulationController&) throw();

}

#endif
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/routing_protocol_keeper.h,v $
* Version $Revision: 1.1 $
* Date    $Date: 2005/10/11 15:24:21 $
*-----------------------------------------------------------------------
* $Log: routing_protocol_keeper.h,v $
 *-----------------------------------------------------------------------*/

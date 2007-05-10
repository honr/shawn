/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_ROUTING_PROTOCOL_FACTORY_INIT_H
#define __SHAWN_APPS_ROUTING_PROTOCOL_FACTORY_INIT_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "sys/simulation/simulation_controller.h"
#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/defutils.h"
#include "apps/routing/routing.h"

#include <string>

namespace shawn
{ 
    class TransmissionModel;
    class SimulationController; 
}

namespace routing 
{

    DECLARE_HANDLES(RoutingProtocolFactory);

    ///
    /* 
    * 
    */
    class RoutingProtocolFactory
        : public shawn::KeeperManaged
    {
    public:
        virtual ~RoutingProtocolFactory();

        virtual RoutingProtocolBase* create(std::string name, shawn::SimulationController& ) throw() = 0;
    };

}

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/routing_protocol_factory.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: routing_protocol_factory.h,v $
 *-----------------------------------------------------------------------*/

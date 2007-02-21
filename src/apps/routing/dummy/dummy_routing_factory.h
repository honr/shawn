/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_ROUTING_DUMMY_ROUTING_FACTORY_H
#define __SHAWN_APPS_ROUTING_DUMMY_ROUTING_FACTORY_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include <string>
#include "apps/routing/routing.h"
#include "apps/routing/routing_protocol_factory.h"

namespace routing 
{
    namespace dummy 
    {
        class DummyRoutingProtocolFactory : public routing::RoutingProtocolFactory
        {
        public:
            virtual ~DummyRoutingProtocolFactory ();
            virtual routing::RoutingProtocolBase* create(std::string name, shawn::SimulationController& ) throw();
            virtual std::string name() const throw();
            virtual std::string description() const throw();
        };
    }
}

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/dummy/dummy_routing_factory.h,v $
* Version $Revision: 1.1 $
* Date    $Date: 2005/10/11 15:24:21 $
*-----------------------------------------------------------------------
* $Log: dummy_routing_factory.h,v $
 *-----------------------------------------------------------------------*/

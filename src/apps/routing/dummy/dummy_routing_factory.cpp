/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "dummy_routing_factory.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/dummy/dummy_routing_factory.h"
#include "apps/routing/dummy/dummy_routing.h"

namespace routing 
{
    namespace dummy
    {

        //-----------------------------------------------------------------------
        DummyRoutingProtocolFactory::
            ~DummyRoutingProtocolFactory ()
        {}

        //-----------------------------------------------------------------------
        routing::RoutingProtocolBase* 
            DummyRoutingProtocolFactory::
            create(std::string name, shawn::SimulationController& ) 
            throw()
        {
            return new DummyRoutingProtocol(name);
        }

        //-----------------------------------------------------------------------
        std::string 
            DummyRoutingProtocolFactory::
            name() 
            const throw()
        {
            return "dummy";
        }

        //-----------------------------------------------------------------------
        std::string 
            DummyRoutingProtocolFactory::
            description() 
            const throw()
        {
            return "Dummy routing protocol factory";
        }

    }
}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/dummy/dummy_routing_factory.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: dummy_routing_factory.cpp,v $
 *-----------------------------------------------------------------------*/

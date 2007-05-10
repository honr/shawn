/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "flood_routing_factory.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/flooding/flood_routing_processor_factory.h"
#include "sys/world.h"
#include "apps/routing/flooding/flood_routing_factory.h"
#include "apps/routing/flooding/flood_routing.h"

namespace routing 
{
    namespace flood
    {

        //-----------------------------------------------------------------------
        FloodRoutingProtocolFactory::
            ~FloodRoutingProtocolFactory ()
        {}

        //-----------------------------------------------------------------------
        routing::RoutingProtocolBase* 
            FloodRoutingProtocolFactory::
            create(std::string name, shawn::SimulationController& sc) 
            throw()
        {
            sc.world_w().add_default_processor( FloodRoutingProcessorFactory::PROCESSOR_NAME );
            return new FloodRoutingProtocol(name);
        }

        //-----------------------------------------------------------------------
        std::string 
            FloodRoutingProtocolFactory::
            name() 
            const throw()
        {
            return "flood";
        }

        //-----------------------------------------------------------------------
        std::string 
            FloodRoutingProtocolFactory::
            description() 
            const throw()
        {
            return "Flood routing protocol factory";
        }

    }
}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/flooding/flood_routing_factory.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: flood_routing_factory.cpp,v $
 *-----------------------------------------------------------------------*/

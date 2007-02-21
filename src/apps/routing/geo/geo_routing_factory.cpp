/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/geo/geo_routing_processor_factory.h"
#include "apps/routing/geo/geo_routing_factory.h"
#include "apps/routing/geo/geo_routing_protocol.h"

namespace routing 
{
    namespace geo
    {

        //-----------------------------------------------------------------------
        GeoRoutingProtocolFactory::
            GeoRoutingProtocolFactory ()
        {}

        //-----------------------------------------------------------------------
        GeoRoutingProtocolFactory::
            ~GeoRoutingProtocolFactory ()
        {}

        //-----------------------------------------------------------------------
        routing::RoutingProtocolBase* 
            GeoRoutingProtocolFactory::
            create(std::string name, shawn::SimulationController& sc) 
            throw()
        {
            sc.world_w().add_default_processor( GeoRoutingProcessorFactory::PROCESSOR_NAME );            
            return new GeoRoutingProtocol(name);
        }

        //-----------------------------------------------------------------------
        std::string 
            GeoRoutingProtocolFactory::
            name() 
            const throw()
        {
            return "geo";
        }

        //-----------------------------------------------------------------------
        std::string 
            GeoRoutingProtocolFactory::
            description() 
            const throw()
        {
            return "Geo routing protocol factory";
        }

    }
}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/geo/geo_routing_factory.cpp,v $
* Version $Revision: 1.1 $
* Date    $Date: 2007/01/16 15:58:07 $
*-----------------------------------------------------------------------
* $Log: geo_routing_factory.cpp,v $
 *-----------------------------------------------------------------------*/

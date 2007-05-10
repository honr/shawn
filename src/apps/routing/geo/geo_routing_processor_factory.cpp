/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include <iostream>

#include "sys/simulation/simulation_controller.h"
#include "sys/processors/processor_keeper.h"

#include "apps/routing/geo/geo_routing_processor_factory.h"
#include "apps/routing/geo/geo_routing_processor.h"

using namespace std;
using namespace shawn;

namespace routing
{
    namespace geo
    {
        const std::string GeoRoutingProcessorFactory::PROCESSOR_NAME = "apps_routing_geo_processor";
        
        // ----------------------------------------------------------------------
        GeoRoutingProcessorFactory::
            GeoRoutingProcessorFactory()
        {}
            
        // ----------------------------------------------------------------------
        GeoRoutingProcessorFactory::
            ~GeoRoutingProcessorFactory()
        {}
        
        // ----------------------------------------------------------------------
        shawn::Processor*
            GeoRoutingProcessorFactory::
            create( void )
            throw()
        {
            return new GeoRoutingProcessor;
        }
            
        // ----------------------------------------------------------------------
        std::string
            GeoRoutingProcessorFactory::
            name( void )
            const throw()
        { 
            return PROCESSOR_NAME; 
        }
        
        // ----------------------------------------------------------------------
        std::string 
            GeoRoutingProcessorFactory::
            description( void )
            const throw()
        {
            return "Internal use for geo routing only";
        }
        
        // ----------------------------------------------------------------------
        void
            GeoRoutingProcessorFactory::
            register_factory( SimulationController& sc )
            throw()
        {
            sc.processor_keeper_w().add( new GeoRoutingProcessorFactory );
        }
    }
}
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/geo/geo_routing_processor_factory.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: geo_routing_processor_factory.cpp,v $
 *-----------------------------------------------------------------------*/

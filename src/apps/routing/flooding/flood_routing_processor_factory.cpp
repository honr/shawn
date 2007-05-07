/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "flood_routing_processor_factory.h"
#ifdef ENABLE_ROUTING

#include <iostream>

#include "sys/simulation/simulation_controller.h"
#include "sys/processors/processor_keeper.h"
#include "apps/routing/flooding/flood_routing_processor_factory.h"
#include "apps/routing/flooding/flood_routing_processor.h"

using namespace std;
using namespace shawn;

namespace routing
{
    namespace flood 
    {
        const std::string FloodRoutingProcessorFactory::PROCESSOR_NAME = "apps_routing_flood_processor";

        // ----------------------------------------------------------------------
        void
            FloodRoutingProcessorFactory::
            register_factory( SimulationController& sc )
            throw()
        {
            sc.processor_keeper_w().add( new FloodRoutingProcessorFactory );
        }

        // ----------------------------------------------------------------------
        FloodRoutingProcessorFactory::
            FloodRoutingProcessorFactory()
        {}

        // ----------------------------------------------------------------------
        FloodRoutingProcessorFactory::
            ~FloodRoutingProcessorFactory()
        {}

        // ----------------------------------------------------------------------
        std::string
            FloodRoutingProcessorFactory::
            name( void )
            const throw()
        { 
			return PROCESSOR_NAME; 
		}

        // ----------------------------------------------------------------------
        std::string 
            FloodRoutingProcessorFactory::
            description( void )
            const throw()
        {
            return "Internal use for flood routing only";
        }

        // ----------------------------------------------------------------------
        shawn::Processor*
            FloodRoutingProcessorFactory::
            create( void )
            throw()
        {
            return new FloodRoutingProcessor;
        }
    }
}
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/flooding/flood_routing_processor_factory.cpp,v $
* Version $Revision: 1.1 $
* Date    $Date: 2005/10/11 15:24:21 $
*-----------------------------------------------------------------------
* $Log: flood_routing_processor_factory.cpp,v $
 *-----------------------------------------------------------------------*/

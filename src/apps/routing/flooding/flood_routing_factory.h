/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_ROUTING_FLOOD_ROUTING_FACTORY_H
#define __SHAWN_APPS_ROUTING_FLOOD_ROUTING_FACTORY_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/routing.h"
#include "apps/routing/routing_protocol_factory.h"
#include <string>

namespace routing 
{
    namespace flood
    {

        //-----------------------------------------------------------------------
        /** Creates instances of the routing::flood::FloodRoutingProtocol.
		  * This protocol does not require any parameterization
          */        
		class FloodRoutingProtocolFactory : public routing::RoutingProtocolFactory
        {
        public:
            virtual ~FloodRoutingProtocolFactory ();

			///Creates an flooding instance with the given name. Also adds a default processor to the
			///world (FloodRoutingProcessorFactory::PROCESSOR_NAME)
            virtual routing::RoutingProtocolBase* create(std::string name, shawn::SimulationController& ) throw();

			///Returns "flood"
            virtual std::string name() const throw();
            virtual std::string description() const throw();
        };
    }
}

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/flooding/flood_routing_factory.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: flood_routing_factory.h,v $
 *-----------------------------------------------------------------------*/

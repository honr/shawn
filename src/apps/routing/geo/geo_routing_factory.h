/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_ROUTING_GEO_ROUTING_FACTORY_H
#define __SHAWN_APPS_ROUTING_GEO_ROUTING_FACTORY_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/routing.h"
#include "apps/routing/routing_protocol_factory.h"
#include <string>

namespace routing 
{
    namespace geo 
    {

        /** Creates new instances of a geo routing protocol instance
            Parameters passed (via the shawn::SimulationEnvironment) to the protocol may be:
          */
        class GeoRoutingProtocolFactory : public routing::RoutingProtocolFactory
        {
        public:
            
            //---------------------------------------------------------------------
            ///@name construction / destruction / factory implementation
            ///@{        
				
				///Construction
				GeoRoutingProtocolFactory();

                ///Destruction
                virtual ~GeoRoutingProtocolFactory ();
            
                ///Factory method. See Class description for parameterization
                virtual routing::RoutingProtocolBase* create(std::string name, shawn::SimulationController& ) throw();
        
            ///@}
        
            //---------------------------------------------------------------------
            ///@name information
            ///@{        
        
                ///Returns the name of this Factory
                virtual std::string name() const throw();
                ///Returns a human readable description
                virtual std::string description() const throw();
        
            ///@}
        };
    }
}

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/geo/geo_routing_factory.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: geo_routing_factory.h,v $
 *-----------------------------------------------------------------------*/

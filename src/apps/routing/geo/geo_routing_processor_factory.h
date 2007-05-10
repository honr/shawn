/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_ROUTING_GEO_ROUTING_PROCESSOR_FACTORY_H
#define __SHAWN_APPS_ROUTING_GEO_ROUTING_PROCESSOR_FACTORY_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "sys/processors/processor_factory.h"
#include <string>

namespace shawn 
{ 
    class SimulationController; 
    class shawn::Processor;
}

namespace routing
{
    namespace geo 
    {

        /** Create instances of the GeoRoutingProcessor class. 
          */
        class GeoRoutingProcessorFactory
            : public shawn::ProcessorFactory
        {
        public:
            
            //---------------------------------------------------------------------
            ///@name construction / destruction
            ///@{        
                GeoRoutingProcessorFactory();
                virtual ~GeoRoutingProcessorFactory();
            ///@}

            //---------------------------------------------------------------------
            ///@name construction / destruction
            ///@{        
                
                ///Creates new instances of GeoRoutingProcessor
                virtual shawn::Processor* create( void ) throw();

                ///Returns the name of this factory
                virtual std::string name( void ) const throw();

                ///A (human readable) description of this class
                virtual std::string description( void ) const throw();
            ///@}

            ///A static helper that registers this factory with the shawn::SimulationController
            static void register_factory( shawn::SimulationController& ) throw();
            ///The name of the factory used by register_factory() and name()
            static const std::string PROCESSOR_NAME;
        };
    }
}

#endif
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/geo/geo_routing_processor_factory.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: geo_routing_processor_factory.h,v $
 *-----------------------------------------------------------------------*/

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_ROUTING_FLOOF_ROUTING_PROCESSOR_FACTORY_H
#define __SHAWN_APPS_ROUTING_FLOOF_ROUTING_PROCESSOR_FACTORY_H

#include "_apps_enable_cmake.h"
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
    namespace flood 
    {

        //-----------------------------------------------------------------------
        /** Creates instances of the internal routing::flood::FloodRoutingProcessor, should
		  * not be used by externally.
          */        
        class FloodRoutingProcessorFactory
            : public shawn::ProcessorFactory
        {
        public:
            FloodRoutingProcessorFactory();
            virtual ~FloodRoutingProcessorFactory();

            virtual std::string name( void ) const throw();
            
			virtual std::string description( void ) const throw();

            virtual shawn::Processor* create( void ) throw();

			///Registers this factory with the corresponding keeper
            static void register_factory( shawn::SimulationController& ) throw();

			///The name of this processor factory
            static const std::string PROCESSOR_NAME;
        };
    }
}

#endif
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/flooding/flood_routing_processor_factory.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: flood_routing_processor_factory.h,v $
 *-----------------------------------------------------------------------*/

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_FLOOD_FLOOD_ROUTING_FACTORY_H
#define __SHAWN_APPS_ROUTING_FLOOD_FLOOD_ROUTING_FACTORY_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "sys/processors/processor_factory.h"

#include "apps/routing/routing_factory.h"

namespace routing
{
	namespace flood
	{
		class FloodRoutingProcessorFactory : public shawn::ProcessorFactory
		{
		public:
			FloodRoutingProcessorFactory();
			virtual ~FloodRoutingProcessorFactory();

			virtual std::string name( void ) const throw();
			virtual std::string description( void ) const throw();
			virtual shawn::Processor* create( void ) throw();

			static void register_factory( shawn::SimulationController& sc ) throw();
			static const std::string PROCESSOR_NAME;
		};// End of FloodRoutingProcessorFactory

		class FloodRoutingFactory : public routing::RoutingFactory
		{
		public:
			virtual ~FloodRoutingFactory();

			virtual routing::RoutingBase* create( const std::string& name, shawn::SimulationController& sc ) throw();
			virtual std::string name() const throw();
			virtual std::string description() const throw();
			static const std::string FACTORY_NAME;
		};// End of FloodRoutingFactory
	}
}

#endif
#endif


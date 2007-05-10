/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_FLOOD_ROUTING_PROCESSOR_H
#define __SHAWN_APPS_ROUTING_FLOOD_ROUTING_PROCESSOR_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/routing.h"
#include "sys/processor.h"
#include <string>

namespace routing 
{
    namespace flood 
    {
		//-----------------------------------------------------------------------
        /** Helper class of the FloodRoutingProtocol to receive messages on each node and
		  * dispatch them to the corresponding instance of the flood routing protocol's
		  * FloodRoutingProtocol::process_message(shawn::Node&, const FloodRoutingMessage&, FloodRoutingInfo& )
		  * remaining hops for the current flooding process. A handle to a 
		  * RoutedMessageInfo instance is kept as well. This is shared between all
		  * FloodRoutingMessages that participate in this flood wave. 
          */
		class FloodRoutingProcessor : public shawn::Processor
        {
        public:
		  FloodRoutingProcessor();

		  ///Invoked externally when a messages is rxed
          virtual bool process_message( const shawn::ConstMessageHandle& ) throw();
        };
    }
}

#endif
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/flooding/flood_routing_processor.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: flood_routing_processor.h,v $
 *-----------------------------------------------------------------------*/

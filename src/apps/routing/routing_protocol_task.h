/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_PROTOCOL_TASK_INIT_H
#define __SHAWN_APPS_ROUTING_PROTOCOL_TASK_INIT_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"

namespace shawn { class SimulationController; }

namespace routing 
{

	//-----------------------------------------------------------------------
    /** Creates a new routing protocol instance with $name of the type $protocol and 
      * places this instance in the routing protocol keeper. This keeper can be obtained
      * using the method routing::routing_protocol_keeper() and routing::routing_protocol_keeper_w().
	  * 
	  * Example use:
	  * <code>
	  *		create_routing_protocol name=my_app_routing protocol=flood
	  * </code>
  	  */
	class RoutingProtocolTask
        : public shawn::SimulationTask
    {
    public:
        RoutingProtocolTask();
        virtual ~RoutingProtocolTask();
        virtual std::string name( void ) const throw();
        virtual std::string description( void ) const throw();
        virtual void run( shawn::SimulationController& sc ) throw( std::runtime_error );
    };

}

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/routing_protocol_task.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: routing_protocol_task.h,v $
 *-----------------------------------------------------------------------*/

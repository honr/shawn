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

namespace shawn
{ class SimulationController; }

namespace routing 
{

    class RoutingProtocolTask
        : public shawn::SimulationTask
    {
    public:
        RoutingProtocolTask();
        virtual ~RoutingProtocolTask();

        virtual std::string name( void ) const throw();
        virtual std::string description( void ) const throw();

        virtual void run( shawn::SimulationController& sc )
            throw( std::runtime_error );

    };



}

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/routing_protocol_task.h,v $
* Version $Revision: 1.1 $
* Date    $Date: 2005/10/11 15:24:22 $
*-----------------------------------------------------------------------
* $Log: routing_protocol_task.h,v $
 *-----------------------------------------------------------------------*/

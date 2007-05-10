/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_NODE_MOVEMENTS_MOVEMENT_RESET_TASK
#define __SHAWN_SYS_NODE_MOVEMENTS_MOVEMENT_RESET_TASK

#include "sys/simulation/simulation_task.h"

namespace shawn
{
    class SimulationController;

    class MovementResetTask
        : public SimulationTask
    {
    public:

        ///@name construction / destruction
        ///@{
        MovementResetTask();
        virtual ~MovementResetTask();
        ///@}

        ///@name task implementation
        ///@{
        virtual void run( SimulationController& ) throw( std::runtime_error );
        virtual std::string name( void ) const throw();
        virtual std::string description( void ) const throw();
        ///@}
    };

}

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/node_movements/playback/movement_reset_task.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: movement_reset_task.h,v $
 *-----------------------------------------------------------------------*/

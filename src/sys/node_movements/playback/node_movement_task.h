/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_NODE_MOVEMENTS_NODE_MOVEMENT_TASK
#define __SHAWN_SYS_NODE_MOVEMENTS_NODE_MOVEMENT_TASK

#include "sys/simulation/simulation_task.h"

namespace shawn
{
    class SimulationController;

    ///Appends a transmission model to the current chain of transmission models
    /** If the last transmission model in the chain is not of type ChainableTransmissionModel
    * the task fails with a std::runtime_error
    */
    class SimulationTaskNodeMovement
        : public SimulationTask
    {
    public:

        ///@name construction / destruction
        ///@{
        SimulationTaskNodeMovement();
        virtual ~SimulationTaskNodeMovement();
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
 * Source  $Source: /cvs/shawn/shawn/sys/node_movements/playback/node_movement_task.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: node_movement_task.h,v $
 *-----------------------------------------------------------------------*/

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/transm_models/stats_chain_transmission_model.h"
#include "sys/transm_models/stats_chain_transmission_model_factory.h"
#include "sys/transm_models/transmission_model_keeper.h"
#include "sys/simulation/simulation_controller.h"

namespace shawn
{

    StatsChainTransmissionModelFactory::
        StatsChainTransmissionModelFactory()
    {}
    // ----------------------------------------------------------------------
    StatsChainTransmissionModelFactory::
        ~StatsChainTransmissionModelFactory()
    {}
    // ----------------------------------------------------------------------
    std::string
        StatsChainTransmissionModelFactory::
        name( void )
        const throw()
    {
        return std::string("stats_chain");
    }
    // ----------------------------------------------------------------------
    std::string
        StatsChainTransmissionModelFactory::
        description( void ) 
        const throw()
    {
        return std::string("A chainable transmission model for message statistics");
    }
    // ----------------------------------------------------------------------
    TransmissionModel*
        StatsChainTransmissionModelFactory::
        create( const SimulationController& )
        throw()
    {
        return new StatsChainTransmissionModel;
    }

}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/stats_chain_transmission_model_factory.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: stats_chain_transmission_model_factory.cpp,v $
 *-----------------------------------------------------------------------*/

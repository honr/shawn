/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TRANSM_MODELS_CSMA_TRANSMISSION_MODEL_FACTORY_H
#define __SHAWN_SYS_TRANSM_MODELS_CSMA_TRANSMISSION_MODEL_FACTORY_H

#include <string>

#include "sys/transm_models/transmission_model_factory.h"

namespace shawn
{

    ///Creates instances of AlohaTransmissionModel
    class CsmaTransmissionModelFactory
        : public TransmissionModelFactory
    {
    public:
        CsmaTransmissionModelFactory();
        virtual ~CsmaTransmissionModelFactory();
        virtual std::string name( void ) const throw();
        virtual std::string description( void ) const throw();
        virtual TransmissionModel* create( const SimulationController& ) throw();
    };

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/csma_transmission_model_factory.h,v $
* Version $Revision: 1.2 $
* Date    $Date: 2005/08/05 10:00:43 $
*-----------------------------------------------------------------------
* $Log: csma_transmission_model_factory.h,v $
 *-----------------------------------------------------------------------*/

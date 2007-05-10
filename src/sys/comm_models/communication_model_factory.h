/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_COMM_MODELS_COMM_MODEL_FACTORY_H
#define __SHAWN_SYS_COMM_MODELS_COMM_MODEL_FACTORY_H

#include <string>

#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/defutils.h"


namespace shawn
{
   class CommunicationModel;
   class SimulationController;

   DECLARE_HANDLES(CommunicationModelFactory);

   /// Abstract parent class for communication model factories. 
   /* Implement a factory class for each communication model you implement. Then register the factory 
    * with the CommunicationModelKeeper in the init method init_communication_models().
    */
   class CommunicationModelFactory
      : public KeeperManaged
   {
   public:

      virtual ~CommunicationModelFactory();
      virtual CommunicationModel* create( const SimulationController& ) const throw() = 0;

   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/comm_models/communication_model_factory.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: communication_model_factory.h,v $
 *-----------------------------------------------------------------------*/

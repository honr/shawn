/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_EDGE_MODELS_EDGE_MODEL_FACTORY_H
#define __SHAWN_SYS_EDGE_MODELS_EDGE_MODEL_FACTORY_H

#include <string>

#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/defutils.h"


namespace shawn
{
   class EdgeModel;
   class SimulationController;

   DECLARE_HANDLES(EdgeModelFactory);

   class EdgeModelFactory
      : public KeeperManaged
   {
   public:

      virtual ~EdgeModelFactory();

      virtual EdgeModel* create( const SimulationController& ) throw() = 0;
   };



}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/edge_models/edge_model_factory.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: edge_model_factory.h,v $
 *-----------------------------------------------------------------------*/

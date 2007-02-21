/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_EDGE_MODELS_LAZY_EDGE_MODEL_FACTORY_H
#define __SHAWN_SYS_EDGE_MODELS_LAZY_EDGE_MODEL_FACTORY_H

#include <string>

#include "sys/edge_models/edge_model_factory.h"


namespace shawn
{

   class LazyEdgeModelFactory
      : public EdgeModelFactory
   {
   public:
      LazyEdgeModelFactory();
      virtual ~LazyEdgeModelFactory();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
      virtual EdgeModel* create( const SimulationController& ) throw();
   };



}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/edge_models/lazy_edge_model_factory.h,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2005/08/05 10:00:38 $
 *-----------------------------------------------------------------------
 * $Log: lazy_edge_model_factory.h,v $
 *-----------------------------------------------------------------------*/

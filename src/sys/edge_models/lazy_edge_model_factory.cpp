/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/edge_models/lazy_edge_model_factory.h"
#include "sys/edge_models/lazy_edge_model.h"
#include "sys/edge_models/edge_model_keeper.h"
#include "sys/simulation/simulation_controller.h"

namespace shawn
{

   LazyEdgeModelFactory::
   LazyEdgeModelFactory()
   {}
   // ----------------------------------------------------------------------
   LazyEdgeModelFactory::
   ~LazyEdgeModelFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   LazyEdgeModelFactory::
   name( void )
      const throw()
   {
      return std::string("simple");
   }
   // ----------------------------------------------------------------------
   std::string
   LazyEdgeModelFactory::
   description( void ) 
      const throw()
   {
      return std::string("A LazyEdgeModel (\"simple\") does not do any caching, but rather iterates over all nodes and repeatedly queries the communication model");
   }
   // ----------------------------------------------------------------------
   EdgeModel*
   LazyEdgeModelFactory::
   create( const SimulationController& )
      throw()
   {
      return new LazyEdgeModel;
   }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/edge_models/lazy_edge_model_factory.cpp,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2005/08/05 10:00:38 $
 *-----------------------------------------------------------------------
 * $Log: lazy_edge_model_factory.cpp,v $
 *-----------------------------------------------------------------------*/

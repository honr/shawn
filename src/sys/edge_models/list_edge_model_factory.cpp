/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/edge_models/list_edge_model_factory.h"
#include "sys/edge_models/list_edge_model.h"
#include "sys/edge_models/edge_model_keeper.h"
#include "sys/simulation/simulation_controller.h"

namespace shawn
{

   ListEdgeModelFactory::
   ListEdgeModelFactory()
   {}
   // ----------------------------------------------------------------------
   ListEdgeModelFactory::
   ~ListEdgeModelFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   ListEdgeModelFactory::
   name( void )
      const throw()
   {
      return std::string("list");
   }
   // ----------------------------------------------------------------------
   std::string
   ListEdgeModelFactory::
   description( void ) 
      const throw()
   {
      return std::string("A ListEdgeModel");
   }
   // ----------------------------------------------------------------------
   EdgeModel*
   ListEdgeModelFactory::
   create( const SimulationController& )
      throw()
   {
      return new ListEdgeModel;
   }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/edge_models/list_edge_model_factory.cpp,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2005/08/05 10:00:38 $
 *-----------------------------------------------------------------------
 * $Log: list_edge_model_factory.cpp,v $
 *-----------------------------------------------------------------------*/

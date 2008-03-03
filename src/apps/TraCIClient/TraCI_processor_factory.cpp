/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TRACINODEMOVEMENTCREATOR

#include <iostream>

#include "sys/processors/processor_keeper.h"
#include "TraCI_processor_factory.h"
#include "TraCI_processor.h"
#include "sys/simulation/simulation_controller.h"

using namespace std;
using namespace shawn;

namespace traci
{

   void
   TraCIProcessorFactory::
   register_factory( SimulationController& sc )
      throw()
   {
      sc.processor_keeper_w().add( new TraCIProcessorFactory );
   }
   // ----------------------------------------------------------------------
   TraCIProcessorFactory::
   TraCIProcessorFactory()
   {
      cout << "TraCIProcessorFactory ctor" << endl;
   }
   // ----------------------------------------------------------------------
   TraCIProcessorFactory::
   ~TraCIProcessorFactory()
   {
      cout << "TraCIProcessorFactory dtor" << endl;
   }
   // ----------------------------------------------------------------------
   std::string
   TraCIProcessorFactory::
   name( void )
      const throw()
   { return "TraCI"; }
   // ----------------------------------------------------------------------
   std::string 
   TraCIProcessorFactory::
   description( void )
      const throw()
   {
	   return "Standard processor for TraCI. \
			  Note: Every node is automatically equipped with this processor while using TraCI.";
   }
   // ----------------------------------------------------------------------
   shawn::Processor*
   TraCIProcessorFactory::
   create( void )
      throw()
   {
      return new TraCIProcessor;
   }


}

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/helloworld/helloworld_processor_factory.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: helloworld_processor_factory.cpp,v $
 *-----------------------------------------------------------------------*/

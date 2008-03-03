/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_LEGACYAPPS_TRACI_PROCESSOR_FACTORY_H
#define __SHAWN_LEGACYAPPS_TRACI_PROCESSOR_FACTORY_H

//#include "config/helloworld.h"
#ifdef ENABLE_TRACINODEMOVEMENTCREATOR

#include "sys/processors/processor_factory.h"

namespace shawn 
{ 
    class SimulationController; 
    class shawn::Processor;
}


namespace traci
{

   class TraCIProcessorFactory
      : public shawn::ProcessorFactory
   {
   public:
      TraCIProcessorFactory();
      virtual ~TraCIProcessorFactory();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
      virtual shawn::Processor* create( void ) throw();

      static void register_factory( shawn::SimulationController& ) throw();
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/helloworld/helloworld_processor_factory.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: helloworld_processor_factory.h,v $
 *-----------------------------------------------------------------------*/

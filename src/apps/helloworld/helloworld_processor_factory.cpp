/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_HELLOWORLD

#include <iostream>

#include "sys/processors/processor_keeper.h"
#include "apps/helloworld/helloworld_processor_factory.h"
#include "apps/helloworld/helloworld_processor.h"
#include "sys/simulation/simulation_controller.h"

using namespace std;
using namespace shawn;

namespace helloworld
{

   void
   HelloworldProcessorFactory::
   register_factory( SimulationController& sc )
      throw()
   {
      sc.processor_keeper_w().add( new HelloworldProcessorFactory );
   }
   // ----------------------------------------------------------------------
   HelloworldProcessorFactory::
   HelloworldProcessorFactory()
   {
      //cout << "HelloworldProcessorFactory ctor" << &auto_reg_ << endl;
   }
   // ----------------------------------------------------------------------
   HelloworldProcessorFactory::
   ~HelloworldProcessorFactory()
   {
      //cout << "HelloworldProcessorFactory dtor" << endl;
   }
   // ----------------------------------------------------------------------
   std::string
   HelloworldProcessorFactory::
   name( void )
      const throw()
   { return "helloworld"; }
   // ----------------------------------------------------------------------
   std::string 
   HelloworldProcessorFactory::
   description( void )
      const throw()
   {
      return "simple HelloWorld demo processor";
   }
   // ----------------------------------------------------------------------
   shawn::Processor*
   HelloworldProcessorFactory::
   create( void )
      throw()
   {
      return new HelloworldProcessor;
   }


}

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/helloworld/helloworld_processor_factory.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: helloworld_processor_factory.cpp,v $
 *-----------------------------------------------------------------------*/

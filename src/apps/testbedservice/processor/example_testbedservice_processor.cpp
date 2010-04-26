/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE
#include "apps/testbedservice/processor/example_testbedservice_processor.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/node.h"
#include "sys/taggings/basic_tags.h"


namespace testbedservice
{
   ExampleTestbedServiceProcessor::
   ExampleTestbedServiceProcessor()
   {}
   // ----------------------------------------------------------------------
   ExampleTestbedServiceProcessor::
   ~ExampleTestbedServiceProcessor()
   {}
   // ----------------------------------------------------------------------
   void
   ExampleTestbedServiceProcessor::
   boot()
      throw()
   {
      // important to call boot of parent!
      TestbedServiceProcessor::boot();

      std::cout << "ExampleTestbedServiceProcessor::boot" << std::endl;
   }
   // ----------------------------------------------------------------------
   void
   ExampleTestbedServiceProcessor::
   process_text_message( TextMessage& message )
      throw()
   {
      std::cout << "ExampleTestbedServiceProcessor::process_text" << std::endl;
      std::cout << "   me  : " << owner().label() << std::endl;
      std::cout << "   from: " << message.source << std::endl;
      std::cout << "   at  : " << message.timestamp << std::endl;
      std::cout << "   msg : " << message.message << std::endl;
      std::cout << "   lev : " << message.level << std::endl;

      std::string reply("my reply!");
      send_text_message( reply, MESSAGE_LEVEL_DEBUG );
   }
   // ----------------------------------------------------------------------
   void
   ExampleTestbedServiceProcessor::
   process_binary_message( BinaryMessage& message )
      throw()
   {
      std::cout << "ExampleTestbedServiceProcessor::process_text" << std::endl;
   }

}
#endif

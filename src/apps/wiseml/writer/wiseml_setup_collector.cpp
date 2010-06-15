/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/writer/wiseml_setup_collector.h"
#ifdef ENABLE_WISEML
namespace wiseml
{
   WisemlSetupCollector::WisemlSetupCollector(shawn::SimulationController &sc) 
      :  WisemlDataCollector(sc)
   {}

   WisemlSetupCollector::~WisemlSetupCollector(){}

   std::string WisemlSetupCollector::name()
      const throw()
   {
      return "wiseml_setup";
   }

   std::string WisemlSetupCollector::description()
      const throw()
   {
      return "Setup section data";
   }
}
#endif

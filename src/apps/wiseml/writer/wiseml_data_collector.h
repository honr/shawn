/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISEML_DATA_COLLECTOR_H
#define __SHAWN_APPS_WISEML_DATA_COLLECTOR_H

#include "_apps_enable_cmake.h"
#ifdef	ENABLE_WISEML
#include "sys/util/keeper_managed.h"
#include "sys/simulation/simulation_controller.h"
using namespace shawn;

namespace wiseml
{
   /**
    * Base class for all DataCollectors used to gather all data to
    * generate a WiseML file.
    */
   class WisemlDataCollector
      : public KeeperManaged
   {
   public:
      WisemlDataCollector(shawn::SimulationController &sc);
      virtual ~WisemlDataCollector();
   protected:
      shawn::SimulationController &sc_;
   };
   DECLARE_HANDLES(WisemlDataCollector);
}

#endif
#endif

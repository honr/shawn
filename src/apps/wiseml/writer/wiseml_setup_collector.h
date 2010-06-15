/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISEML_SETUP_COLLECTOR_H
#define __SHAWN_APPS_WISEML_SETUP_COLLECTOR_H

#include "_apps_enable_cmake.h"
#ifdef	ENABLE_WISEML
#include "apps/wiseml/writer/wiseml_data_collector.h"
#include "apps/wiseml/templates.h"
#include "sys/simulation/simulation_controller.h"
using namespace shawn;

namespace wiseml
{
   /**
    * SetupCollector gathers data for the setup section of a wiseml file.
    */
   class WisemlSetupCollector
      : public WisemlDataCollector
   {
   public:
      WisemlSetupCollector(shawn::SimulationController &sc);
      virtual ~WisemlSetupCollector();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
      
      //NodeTemplate* default_node_w();
      //NodeTemplate* default_node() const;
      //void add_node();
      //void add_link();
   };
   DECLARE_HANDLES(WisemlSetupCollector);
}

#endif
#endif

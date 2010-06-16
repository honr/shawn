/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/writer/wiseml_data_keeper.h"
#ifdef ENABLE_WISEML

namespace wiseml
{
   WisemlDataKeeper::WisemlDataKeeper(shawn::SimulationController &sc)
      :  HandleKeeper<WisemlDataCollector>("DataKeeper", "wiseml"),
         sc_(sc)
   {
      setup_ = new WisemlSetupCollector(sc);
      add(setup_);
   }
   WisemlDataKeeper::~WisemlDataKeeper()
   {
   }
}

#endif

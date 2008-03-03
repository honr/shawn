/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "TraCI_init.h"
#ifdef ENABLE_TRACINODEMOVEMENTCREATOR

#include "TraCI_init.h"
#include "TraCI_processor_factory.h"

extern "C" void init_traci( shawn::SimulationController& sc )
{
   std::cout << "TraCI init" << std::endl;
   traci::TraCIProcessorFactory::register_factory(sc);
}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/helloworld/helloworld_init.cpp,v $
 * Version $Revision: 67 $
 * Date    $Date: 2007-09-14 14:53:58 +0200 (Fr, 14 Sep 2007) $
 *-----------------------------------------------------------------------
 * $Log: helloworld_init.cpp,v $
 *-----------------------------------------------------------------------*/

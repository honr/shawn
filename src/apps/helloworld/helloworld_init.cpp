/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "helloworld_init.h"
#ifdef ENABLE_HELLOWORLD

#include "apps/helloworld/helloworld_init.h"
#include "apps/helloworld/helloworld_processor_factory.h"
#include "sys/simulation/simulation_controller.h"

extern "C" void init_helloworld( shawn::SimulationController& sc )
{
   std::cout << "helloworld init" << std::endl;
   helloworld::HelloworldProcessorFactory::register_factory(sc);
}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/helloworld/helloworld_init.cpp,v $
 * Version $Revision: 1.3 $
 * Date    $Date: 2005/08/05 10:00:29 $
 *-----------------------------------------------------------------------
 * $Log: helloworld_init.cpp,v $
 *-----------------------------------------------------------------------*/

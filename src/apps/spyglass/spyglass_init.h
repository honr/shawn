/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_SPYGLASS_SPYGLASS_INIT_H
#define __SHAWN_APPS_SPYGLASS_SPYGLASS_INIT_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_SPYGLASS

namespace shawn
{ class SimulationController; }


extern "C" void init_spyglass( shawn::SimulationController& );


#endif
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/spyglass/spyglass_init.h,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2005/08/05 10:00:33 $
 *-----------------------------------------------------------------------
 * $Log: spyglass_init.h,v $
 *-----------------------------------------------------------------------*/

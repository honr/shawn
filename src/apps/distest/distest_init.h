/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_DIST_ESTINIT_H
#define __SHAWN_APPS_DIST_ESTINIT_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_DISTEST

namespace shawn
{ class SimulationController; }

extern "C" void init_distest( shawn::SimulationController& );
    
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/distest/distest_init.h,v $
 * Version $Revision: 1.3 $
 * Date    $Date: 2005/08/15 15:34:35 $
 *-----------------------------------------------------------------------
 * $Log: distest_init.h,v $
 *-----------------------------------------------------------------------*/

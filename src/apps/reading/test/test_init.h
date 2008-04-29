/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_READING_TEST_INIT_H
#define __SHAWN_APPS_READING_TEST_INIT_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_READING


namespace shawn
{ class SimulationController; }

namespace reading
{
	void init_test_init( shawn::SimulationController& );
}

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/reading/test/test_init.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------*/


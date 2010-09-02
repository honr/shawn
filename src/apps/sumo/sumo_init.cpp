/************************************************************************
 ** Source  $HeadURL: https://www.ldv.ei.tum.de/repos/vertrauenstuds/shawn-apps/trunk/sumo/sumo_init.cpp $
 ** Version $Revision: 416 $
 ** Id      $Id: sumo_init.cpp 416 2010-09-01 22:12:09Z wbam $
 ************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Copyright (C) 2010 Josef Schlittenlacher                           **
 ** Copyright (C) 2010 Technische Unversitaet Muenchen (www.tum.de)    **
 ** All rights reserved.                                               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sumo_init.h"
#ifdef ENABLE_SUMO

#include <iostream>

extern "C" void init_sumo( shawn::SimulationController& sc )
{
	std::cout << "Initialising app SUMO" << std::endl;
}

#endif /* ENABLE_SUMO */

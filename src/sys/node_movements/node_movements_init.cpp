/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/node_movements/node_movements_init.h"
#include "sys/node_movements/playback/playback_init.h"
#include "sys/simulation/simulation_controller.h"


namespace shawn
{

   void init_node_movements( SimulationController& sc ) throw()
   {
      init_playback(sc);
      //init_node_movements_tasks(sc);
   }
}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/node_movements/node_movements_init.cpp,v $
 * Version $Revision: 1.3 $
 * Date    $Date: 2005/08/08 17:46:15 $
 *-----------------------------------------------------------------------

 *-----------------------------------------------------------------------*/

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "shawn_config.h"

#include "sys/xml/xml_init.h"
#include "sys/worlds/save_world_task.h"

#include "sys/simulation/simulation_task_keeper.h"
#include "sys/simulation/simulation_controller.h"


namespace shawn
{

   void 
   init_xml( SimulationController& sc )
      throw()
   {
#ifdef HAVE_EXPAT 
       sc.simulation_task_keeper_w().add( new shawn::SimulationTaskSaveWorld );
#endif
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/xml/xml_init.cpp,v $
 * Version $Revision: 1.5 $
 * Date    $Date: 2005/08/05 10:00:45 $
 *-----------------------------------------------------------------------
 * $Log: xml_init.cpp,v $
 *-----------------------------------------------------------------------*/

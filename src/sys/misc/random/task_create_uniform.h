/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_RANDOM_TASK_CREATE_UNIFORM_H
#define __SHAWN_SYS_MISC_RANDOM_TASK_CREATE_UNIFORM_H

#include "sys/simulation/simulation_task.h"

namespace shawn
{
   class SimulationController;

   class SimulationTaskCreateUniform
      : public SimulationTask
   {
   public:
      SimulationTaskCreateUniform();
      virtual ~SimulationTaskCreateUniform();
      virtual void run( SimulationController& ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/random/task_create_uniform.h,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2005/08/05 10:00:40 $
 *-----------------------------------------------------------------------
 * $Log: task_create_uniform.h,v $
 *-----------------------------------------------------------------------*/

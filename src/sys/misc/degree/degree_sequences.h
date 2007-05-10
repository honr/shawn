/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_TASKS_DEGREE_SEQUENCES_H
#define __SHAWN_SYS_MISC_TASKS_DEGREE_SEQUENCES_H

#include "sys/simulation/simulation_task.h"

namespace shawn
{

   class SimulationTaskDegreeSequence
      : public SimulationTask
   {
   public:
      SimulationTaskDegreeSequence();
      virtual ~SimulationTaskDegreeSequence();
      virtual void run( SimulationController& ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/degree/degree_sequences.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: degree_sequences.h,v $
 *-----------------------------------------------------------------------*/

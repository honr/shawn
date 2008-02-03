
#ifndef __SHAWN_SYS_DISTANCE_ESTIMATES_TASK_POLYGON_DISTANCE_ESTIMATE_H
#define __SHAWN_SYS_DISTANCE_ESTIMATES_TASK_POLYGON_DISTANCE_ESTIMATE_H

#include "sys/simulation/simulation_task.h"

namespace topology{

   class SimulationTaskPolygonDistanceEstimate
	   : public shawn::SimulationTask
   {
   public:
      SimulationTaskPolygonDistanceEstimate();
      virtual ~SimulationTaskPolygonDistanceEstimate();
      virtual void run( shawn::SimulationController& ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
   };

}

#endif

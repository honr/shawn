#include "sys/node_movements/playback/random_direction_node_movement_creator.h"
#include "sys/node_movements/playback/movement_controller.h"
#include "sys/node_movements/playback/movement_info.h"
#include "sys/node_movements/linear_movement.h"
#include "sys/node_movement.h"
#include "sys/node.h"
#include "sys/world.h"
#include <string>
#include <limits>
#include <math.h>

#define RANDOMDIRECTION_STARTNOW -1.0
#define PI 3.14159265

using namespace std;

namespace shawn
{

	//-------------------------------------------------------------------------------
	/**
	 * 
	 */
   	RandomDirectionNodeMovementCreator::
   		RandomDirectionNodeMovementCreator(SimulationController& sc)
   		: sc_(sc)
   	{
		  urvSpeed_.set_lower_bound(sc.environment_w().optional_double_param("random_direction_v_min", 0.001));
		  urvSpeed_.set_upper_bound(sc.environment_w().optional_double_param("random_direction_v_max", 1.0));
		  urvSpeed_.set_lower_bound_inclusive(true);
		  urvSpeed_.set_upper_bound_inclusive(true);
		  urvSpeed_.init();
		
		  urvDirection_.set_lower_bound(0.0);
		  urvDirection_.set_upper_bound(2*PI);
		  urvDirection_.set_lower_bound_inclusive(true);
		  urvDirection_.set_upper_bound_inclusive(false);
		  urvDirection_.init();
		
		  width_ = sc.environment_w().required_double_param("width");
		  height_ = sc.environment_w().required_double_param("height");
   	}

	//-------------------------------------------------------------------------------
	/**
	 * 
	 */
	RandomDirectionNodeMovementCreator::
		~RandomDirectionNodeMovementCreator(void)
   {}


	//-------------------------------------------------------------------------------
	/**
	 * 
	 */
   MovementInfo* 
   	RandomDirectionNodeMovementCreator::
   	next_movement()
    throw( std::runtime_error )
   {
      if (static_cast<int>(next_movement_times_.size()) < sc_.world().node_count())
      {
         // Generate new movements for remaining nodes
         for (World::node_iterator it=sc_.world_w().begin_nodes_w(); it!=sc_.world_w().end_nodes_w(); ++it)
         {
            NodeMovement &nm = it->movement_w();
			// Creating a new (vanilla) nmt_info for new movement
			nmt_info info;
			info.node = &(*it);
			Vec vector;
			info.rest_vector = vector;
			info.rest_speed = 0.0;
            if (nm.name() == "NoMovement") return generateNewMovement(info, RANDOMDIRECTION_STARTNOW);
            LinearMovement *lm = dynamic_cast<LinearMovement*>(&nm);
            if ( lm  && ( lm->position() == lm->destination() ) )
            {
               return generateNewMovement(info, RANDOMDIRECTION_STARTNOW);
            }
         }
      }
      
      // Otherwise get the next scheduled movement
      if (next_movement_times_.size() > 0)
      {
         double startTime = next_movement_times_.begin()->first;
         nmt_info info = next_movement_times_.begin()->second;
         next_movement_times_.erase(next_movement_times_.begin());
         return generateNewMovement(info, startTime);
      }

      return NULL;

   }

	//-------------------------------------------------------------------------------
	/**
	 * 
	 */
   MovementInfo* 
   	RandomDirectionNodeMovementCreator::
	generateNewMovement(nmt_info& info, double startTime)
   {
      // Generate a new movement
      MovementInfo* mi = new MovementInfo();
	  mi->set_node(info.node);
      if (startTime < 0.0) 
      {
         mi->set_urgency(MovementInfo::Immediately);
      } 
      else 
      {
         mi->set_urgency(MovementInfo::Delayed);
         mi->set_dispatch_time(startTime);
      }
      
      LinearMovement* lm = new LinearMovement();

      // Get movement vector
	  NodeMovement &nm = info.node->movement_w();
	  lm = dynamic_cast<LinearMovement*>(&nm);
	  Vec vector;
	  double direction = 0;
	  // Fetching the old speed
	  double speed = info.rest_speed;
	  if ( info.rest_vector.euclidean_norm() == 0 ){
	    // Complete new move
		direction = urvDirection_;
		speed = urvSpeed_;
		Vec v_temp(sin(direction) * speed, cos(direction) * speed, 0.0);
		vector = v_temp;
	  }else{
	    // Scheduled move
		vector = info.rest_vector;
	  }

      double intersectTime = std::numeric_limits<double>::max();

      // Get x intersection
      if (vector.x() < 0)
      {
		 double t = (0 - info.node->real_position().x()) / vector.x();
         if (t < intersectTime) 
        	 intersectTime = t;
      } 
      else if (vector.x() > 0)
      {
         double t = (width_ - info.node->real_position().x()) / vector.x();
         if (t < intersectTime) 
        	 intersectTime = t;
      }
      // Get y intersection
      if (vector.y() < 0)
      {
         double t = (0 - info.node->real_position().y()) / vector.y();
         if (t < intersectTime) 
        	 intersectTime = t;
      } 
      else if (vector.y() > 0)
      {
         double t = (height_ - info.node->real_position().y()) / vector.y();
         if (t < intersectTime) 
        	 intersectTime = t;
      }

	  lm->set_parameters(speed, info.node->real_position() + (vector * intersectTime), sc_.world_w());
      mi->set_node_movement(lm);

	  // Remaining movement
	  double rest_length = (vector * intersectTime).euclidean_norm() - vector.euclidean_norm();
	  double alpha = acos(vector.x()/vector.euclidean_norm());
	  Vec rest_vector(rest_length * cos(2*PI - alpha),rest_length * sin(2*PI - alpha),0.0);
	  // Creating a new nmt_info for the scheduled movement
	  nmt_info new_info;
	  new_info.node = info.node;
	  new_info.rest_vector = rest_vector;
	  new_info.rest_speed = speed;

      // Schedule, when the node will need a new movement
      next_movement_times_.insert(std::make_pair(startTime+intersectTime,new_info));

      return mi;
   }

	//-------------------------------------------------------------------------------
	/**
	 * 
	 */
   void 
   	RandomDirectionNodeMovementCreator::
   	reset()
   {
      next_movement_times_.clear();
   }


}

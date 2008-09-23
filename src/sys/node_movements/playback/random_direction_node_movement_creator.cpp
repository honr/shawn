#include "sys/node_movements/playback/random_direction_node_movement_creator.h"
#include "sys/node_movements/playback/movement_controller.h"
#include "sys/node_movements/playback/movement_info.h"
#include "sys/node_movements/linear_movement.h"
#include "sys/node_movement.h"
#include "sys/node.h"
#include "sys/world.h"
#include <string>
#include <cfloat>
#include <cmath>

#define RANDOMDIRECTION_STARTNOW 0.0
#define PI 3.14159265

using namespace std;

namespace shawn{

   RandomDirectionNodeMovementCreator::RandomDirectionNodeMovementCreator(SimulationController& sc)
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

   RandomDirectionNodeMovementCreator::~RandomDirectionNodeMovementCreator(void)
   {
   }


   MovementInfo* RandomDirectionNodeMovementCreator::next_movement()
      throw( std::runtime_error )
   {
      if (static_cast<int>(next_movement_times_.size()) < sc_.world().node_count())
      {
         // Generate new movements for remaining nodes
         for (World::node_iterator it=sc_.world_w().begin_nodes_w(); it!=sc_.world_w().end_nodes_w(); ++it)
         {
            NodeMovement &nm = it->movement_w();
			if (nm.name() == "NoMovement"){
				std::cerr << "---[RANDOM DIRECTION NODEMOVEMENTCREATOR (new movement 0) Current Node: " << it->id() << " BEGIN]---" << std::endl;
				std::cerr << "Current Time: " << sc_.world().current_time() << std::endl;
				std::cerr << "---[RANDOM DIRECTION NODEMOVEMENTCREATOR (new movement 0) Current Node: " << it->id() << " END]---" << std::endl;
				return generateNewMovement(*it, RANDOMDIRECTION_STARTNOW);
			}
            LinearMovement *lm = dynamic_cast<LinearMovement*>(&nm);
            if ( ( lm != NULL ) & ( lm->position() == lm->destination() ) )
            {
			   std::cerr << "---[RANDOM DIRECTION NODEMOVEMENTCREATOR (new movement 1) Current Node: " << it->id() << " BEGIN]---" << std::endl;
			   std::cerr << "Current Time: " << sc_.world().current_time() << std::endl;
			   std::cerr << "---[RANDOM DIRECTION NODEMOVEMENTCREATOR (new movement 1) Current Node: " << it->id() << " END]---" << std::endl;
               return generateNewMovement(*it, RANDOMDIRECTION_STARTNOW);
            }
         }
      }
      // Otherwise get the next scheduled movement
      if (next_movement_times_.size() > 0)
      {
         double startTime = next_movement_times_.begin()->first;
         Node *n = next_movement_times_.begin()->second;
         assert (n != NULL);
         next_movement_times_.erase(next_movement_times_.begin());
		 std::cerr << "---[RANDOM DIRECTION NODEMOVEMENTCREATOR (new movement 2) Current Node: " << n->id() << " BEGIN]---" << std::endl;
		 std::cerr << "Current Time: " << sc_.world().current_time() << std::endl;
		 std::cerr << "Start Time: " << startTime << std::endl;
		 std::cerr << "---[RANDOM DIRECTION NODEMOVEMENTCREATOR (new movement 2) Current Node: " << n->id() << " END]---" << std::endl;
         return generateNewMovement(*n, startTime);
      }

      return NULL;

   }

   MovementInfo* RandomDirectionNodeMovementCreator::generateNewMovement(Node &node, double startTime)
   {
      // Generate a new movement
      MovementInfo* mi = new MovementInfo();
	  shawn::Vec destination = node.real_position();
	  if (node.movement().name() == "LinearMovement"){
		  const shawn::LinearMovement& lm = dynamic_cast<const shawn::LinearMovement&>(node.movement());
		  destination = lm.destination();
	  }
      mi->set_node(&node);
      if (startTime <= 0.0) 
      {
         mi->set_urgency(MovementInfo::Immediately);
      } else {
         mi->set_urgency(MovementInfo::Delayed);
         mi->set_dispatch_time(startTime);
      }
      LinearMovement* lm = new LinearMovement();

      // Get movement vector
      double direction = urvDirection_;
      double speed = urvSpeed_;
      Vec vector(cos(direction) * speed, sin(direction) * speed, 0.0);
	  double intersection = DBL_MAX;

      // Get x intersection
      if (vector.x() < 0)
      {
		 double t = (0 - destination.x()/*node.real_position().x()*/) / vector.x();
         if (t < intersection) intersection = t;
      } else if (vector.x() > 0){
         double t = (width_ - destination.x()/*node.real_position().x()*/) / vector.x();
         if (t < intersection) intersection = t;
      }
      // Get y intersection
      if (vector.y() < 0)
      {
         double t = (0 - destination.y()/*node.real_position().y()*/) / vector.y();
         if (t < intersection) intersection = t;
      } else if (vector.y() > 0){
         double t = (height_ - destination.y()/*node.real_position().y()*/) / vector.y();
         if (t < intersection) intersection = t;
      }

	  shawn::Vec w = destination/*node.real_position()*/ + (vector * intersection);
	  lm->set_parameters(speed, w, sc_.world_w());
      mi->set_node_movement(lm);
      // Schedule, when the node will need a new movement
	  /*double intersectTime = (vector * intersection).euclidean_norm()/speed*/;
	  std::cerr << "---[RANDOM NODE MOVEMENT CREATOR Current Node: " << node.id() << " BEGIN]---" << std::endl;
	  std::cerr << "Start Time: " << startTime << std::endl;
	  std::cerr << "Real Position: " << node.real_position().x() << "\t" << node.real_position().y() << std::endl;
	  std::cerr << "vector: " << vector.x() << "\t" << vector.y() << std::endl;
	  std::cerr << "intersection: " << intersection << std::endl;
	  std::cerr << "(vector * intersection): " << (vector * intersection).x() << "\t" << (vector * intersection).y() << std::endl;
	  std::cerr << "w (Destination): " << w.x() << "\t" << w.y() << std::endl;
	  std::cerr << "Speed: " << speed << std::endl;
	  /*std::cerr << "(vector * intersection).euclidean_norm(): " << (vector * intersection).euclidean_norm() << std::endl;*/
	  /*std::cerr << "Intersect Time: " << intersectTime << std::endl;*/
	  std::cerr << "---[RANDOM NODE MOVEMENT CREATOR Current Node: " << node.id() << " END]---" << std::endl;
      next_movement_times_.insert(std::make_pair(startTime+intersection/*intersectTime*/,&node));

      return mi;
   }

   void RandomDirectionNodeMovementCreator::reset()
   {
      next_movement_times_.clear();
   }


}

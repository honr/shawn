/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004 by  SwarmNet (www.swarmnet.de)                  **
 **                    and SWARMS   (www.swarms.de)                    **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/

#ifndef __SHAWN_APPS_MOTION_EVENT_TASK_MOTION_EVENT_TASK_H
#define __SHAWN_APPS_MOTION_EVENT_TASK_MOTION_EVENT_TASK_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_MOTIONEVENTTASK

#include "sys/simulation/simulation_task.h"
#include "sys/node.h"
#include <list>
using std::list;
using std::pair;

namespace shawn {
	class Vec;
}

namespace motion_event
{
   class SimulationController;

   /** \brief Simulates moving objects in the world
	*
	* Objects begin to move at the specified time and move with a certain velocity 
	* according to the given positions without stop. The positions are specified 
	* through vectors (x-, y- and z-coordinate). 
	* 
	* Each node in the world has the same detection range (approximated by a circle). 
	* When an object crosses the detection range of one node, the task adds a 
	* "MotionEventTag" to this node witch contains the time of the crossing multiplied
	* with an error.
	* 
	* The given standard deviation (standard_deviation) and maximum error (max_time_error) influence this error.  
	*/
   class MotionEventTask
      : public shawn::SimulationTask
   {
   public:
      MotionEventTask();
      virtual ~MotionEventTask();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

      virtual void run( shawn::SimulationController& sc )
         throw( std::runtime_error );

   private:
	   // Private functions for the computation of the times of detection
	   void single_line(shawn::SimulationController& sc, const shawn::Vec start_pos, const shawn::Vec dest_pos);
	   bool between(const shawn::Vec, const shawn::Vec, const shawn::Vec);
	   bool in_range_of_node(shawn::Node*);
	   void set_in_range_of_node(shawn::Node*, bool);

	   /** \brief Object id
	    */
	   int object_id_;
	   
	   /** \brief Velocity of the object
	    *
	    *  The velcoity of one person does not change in one simulation.
	    */
	   double velocity_;
	   
	   /** \brief Detection range of each nodes in the world.
	    *
	    *  The range of the nodes is approximated by a circle.
	    */
	   double detection_range_;
	   
	   /** \brief Start time of the object.
	    */
	   double start_time_;
	   
	   /** \brief Standard deviation for the error added to (or subtracted from) the time of detection
	    *
	    * The default value is 0. So a nodes detects an object when this is at the nearest point of 
	    * the node. If the standard deviation is greater than 0, a random error is added to this time of 
	    * detection.
	    */
	   double standard_deviation_;
	   
	   /** \brief Maximum of the error added to the time of detection
	    */
	   double max_time_error_;
	   
	   /** \brief Temporary saving the direction change of an object. 
	    */
	   std::list<std::pair<shawn::Node*,bool> > in_range_of_nodes_; 

   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/motion_event_task/motion_event_task.h,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2007/01/31 17:24:17 $
 *-----------------------------------------------------------------------
 * $Log: motion_event_task.h,v $
 * Revision 1.2  2007/01/31 17:24:17  krueger
 * *** empty log message ***
 *
 * Revision 1.1  2007/01/29 16:37:14  krueger
 * *** empty log message ***
 *
 * Revision 1.1  2005/03/05 06:43:10  tbaum
 * added motion_event_task
 *
*-----------------------------------------------------------------------*/

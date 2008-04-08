/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2007 by AutoNomos (www.auto-nomos.de)                **
 ** This part of Shawn is free software; you can redistribute it and/or**
 ** modify it under the terms of the BSD License. Refer to the		   **
 ** shawn-licence.txt file in the root of the Shawn source tree for	   **
 ** further details.												   **
 **                                                                    **
 ** \author Axel Wegener <wegener@itm.uni-luebeck.de>				   **
 ** \author Torsten Teubler <teubler@itm.uni-luebeck.de>			   **
 **                                                                    **
 ************************************************************************/

#include "autocast_task.h"
#ifdef ENABLE_AUTOCAST

#include "apps/autocast/autocast_task/autocast_task.h"
#include "sys/vec.h"
#include "sys/world.h"
#include "sys/event_scheduler.h"
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <cassert>

namespace autocast
{

   using namespace std;

   AutoCastTask::
   AutoCastTask()
   {
	   std::cout << "AutoCastTask ctor" << std::endl;
   }
   // ----------------------------------------------------------------------
   AutoCastTask::
   ~AutoCastTask()
   {
	   std::cout << "AutoCastTask dtor" << std::endl;
   }
   // ----------------------------------------------------------------------
   std::string
   AutoCastTask::
   name( void )
      const throw()
   {
      return "AutoCastTask";
   }
   // ----------------------------------------------------------------------
   std::string
   AutoCastTask::
   description( void )
      const throw()
   {
      return "AutoCastTask";
   }
   // ----------------------------------------------------------------------
   void
   AutoCastTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      require_world( sc );

	  // Write the latencies
	  // Create ns2-like output
	  // latencies: fromAddr	node	timeCreated[s]	latency[s]	distance[m]	latency[s/km]	propagation_speed[km/h]	active
	  {
		  std::string latencies_log_filename = sc.environment().required_string_param("latencies_log_filename");
		  std::ofstream file_op(latencies_log_filename.c_str(),ios::out);
		  file_op << "fromAddr" << "\t"
			      << "node" << "\t"
				  << "timeCreated[s]" << "\t"
				  << "latency[s]" << "\t"
				  << "distance[m]" << "\t"
				  << "latency[s/km]" << "\t"
				  << "propagation_speed[km/h]" << "\t"
				  << "active" << std::endl;
		  for(std::vector<latencies_line*>::iterator it = latencies_.begin(); it != latencies_.end(); it++){
				file_op << (*it)->from_addr << "\t"
						<< (*it)->node_addr << "\t"
						<< (*it)->time << "\t"
						<< (*it)->latency_s << "\t"
						<< (*it)->dist << "\t"
						<< (*it)->latency_s_per_km << "\t"
						<< (*it)->propagation_speed << "\t"
						<< 1 << std::endl;
				// Delete the previous printed line
				delete (*it);
		  }
		  file_op.close();
	  }

	  // Write the sent statistic
	  // Create ns2-like output
	  // sentstat: nodeId	packetsSent	bytesSent	hdcsSent	hdcBytesSent	packetsRcvd	hdcsRcvd	rcvdHDCids	activeTime	startStopTimes
	  {
		  std::string sent_statistics_filename = sc.environment().required_string_param("sent_statistics_filename");
		  std::ofstream file_op(sent_statistics_filename.c_str(),ios::out);
		  file_op << "nodeId" << "\t" 
				  << "packetsSent" << "\t"
				  << "bytesSent" << "\t"
				  << "hdcsSent" << "\t"
				  << "hdcBytesSent" << "\t"
				  << "packetsRcvd" << "\t"
				  << "hdcsRcvd" << "\t"
				  << "rcvdHDCids" << "\t"
				  << "activeTime" << "\t"
				  << "startStopTimes" /*<< "\t"
				  << "neighbors_" << "\t"
				  << "real_neighbors_" << "\t"
				  << "velocity_" << "\t"
				  << "update_time_"*/
				  << std::endl;
		  for(std::vector<sent_statistics_line*>::iterator it = sent_statistics_.begin(); it != sent_statistics_.end(); it++){
				file_op << (*it)->node_addr << "\t"
						<< (*it)->packet_sent << "\t"
						<< (*it)->byte_sent << "\t"
						<< (*it)->dataUnit_sent << "\t"
						<< (*it)->dataUnit_byte_sent << "\t"
						<< (*it)->packets_received << "\t"
						<< (*it)->dataUnits_received << "\t"
						<< (*it)->received_DataUnit_ids << "\t"
						<< 3600.000000 << "\t"
						<< "\"0 3600 \"" /*<< "\t"
						<< (*it)->neighbors_count_ << "\t"
						<< (*it)->real_neighbors_count_ << "\t"
						<< (*it)->velocity_ << "\t"
						<< (*it)->update_time_*/
						<< std::endl;
				// Delete the previous printed line
				delete (*it);
		  }
		  file_op.close();
	  }
	  // Cleaning up!
	  latencies_.clear();
	  sent_statistics_.clear();
   }
   // ----------------------------------------------------------------------
   void 
	   AutoCastTask::
	   process_latencies(const autocast::ConstDataUnitHandle& duh, const shawn::Node* node, const shawn::SimulationController& sc)
	   throw()
   {
		latencies_line * line = new latencies_line();
		line->dist = sqrt( pow((node->real_position().x() - duh->x()),2) + pow((node->real_position().y() - duh->y()),2) );
		line->latency_s = sc.world().scheduler().current_time() - duh->time();
		line->latency_s = (line->latency_s == 0 ? line->dist/299792458 : line->latency_s);
		line->from_addr = duh->from_addr();
		line->node_addr = node->id();
		line->time = duh->time();
		line->latency_s_per_km = (line->latency_s == 0 ? 1/299792458 : (line->latency_s/line->dist)*1000);
		line->propagation_speed = (line->latency_s == 0 ? 299792458 : (line->dist/line->latency_s) * 3.6);
		latencies_.push_back(line);
   }
   // ----------------------------------------------------------------------
   void
	   AutoCastTask::
	   process_sent_statistic(const shawn::Node* node, int packet_sent, int byte_sent , int dataUnit_sent, int dataUnit_byte_sent, int packets_received, int dataUnits_received, int received_DataUnit_ids/*, double neighbors_count, double real_neighbors_count, double velocity, double update_time*/)
	   throw()
   {
	   sent_statistics_line * line = new sent_statistics_line();
	   line->node_addr = node->id();
	   line->packet_sent = packet_sent;
	   line->byte_sent = byte_sent;
	   line->dataUnit_sent = dataUnit_sent;
	   line->dataUnit_byte_sent = dataUnit_byte_sent;
	   line->packets_received = packets_received;
	   line->dataUnits_received = dataUnits_received;
	   line->received_DataUnit_ids = received_DataUnit_ids;
	   /*line->neighbors_count_ = neighbors_count;
	   line->real_neighbors_count_ = real_neighbors_count;
	   line->velocity_ = velocity;
	   line->update_time_ = update_time;*/
	   sent_statistics_.push_back(line);
   }

}
#endif
/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/


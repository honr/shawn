/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/routing_protocol_keeper.h"

#include "sys/node_distance_estimate.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"
#include "sys/node.h"
#include "sys/message.h"

#include "apps/routing/geo/geo_routing.h"
#include "apps/routing/geo/test/geo_observer.h"
#include "apps/routing/geo/test/geo_test.h"

#include <iostream>
using namespace std;

#define UINT_INF ~0U
#define FLOAT_INF 99999.0

namespace routing 
{
  namespace geo 
  {

    // ----------------------------------------------------------------------
    GeoTrace::
      GeoTrace(shawn::Node& sender, GeoRoutingAddress dest, shawn::MessageHandle msg) 
        : sender_(sender), 
          dest_ (dest),
          msg_(msg)
    {}
      
    // ----------------------------------------------------------------------
    NodeListIt 
      GeoTrace::
      path_begin()  
    { 
      return path_.begin(); 
    }
    
    // ----------------------------------------------------------------------
    NodeListIt 
      GeoTrace::
      path_end()    
    { 
      return path_.end(); 
    }
    
    // ----------------------------------------------------------------------
    shawn::Node& 
      GeoTrace::
      sender() 
    { 
      return sender_; 
    }
    
    // ----------------------------------------------------------------------
    const GeoRoutingAddress& 
      GeoTrace::
      destination() 
    { 
      return dest_;
    }

    // ----------------------------------------------------------------------
    shawn::MessageHandle 
      GeoTrace::
      message()
    {
      return msg_;
    }

    // ----------------------------------------------------------------------
    void 
      GeoTrace::
      visited(shawn::Node& node) 
    { 
      path_.push_back(&node); 
    }    

    // ----------------------------------------------------------------------
    // ----------------------------------------------------------------------
    // ----------------------------------------------------------------------
    
    //------------------------------------------------------------------------------------ 
    GeoObserver::
      GeoObserver()
		  : distances_calculated_(false)
    {
    }
      
    // ----------------------------------------------------------------------
    GeoObserver::
      ~GeoObserver()
    {
    }

    // ----------------------------------------------------------------------
    void
		GeoObserver::
      dump_paths(std::ostream& out)
    {
		out << "---------------------------" << endl;
		out << "Dumping observed paths" << endl << endl;
		  
		for( std::map<shawn::MessageHandle, GeoTrace*>::iterator it = traces_.begin(), end = traces_.end(); it!=end; ++it)
		{
			 out << "Path: ";
			 for( NodeListIt pit = it->second->path_begin(), pend = it->second->path_end(); pit!=pend; ++pit)
				out << (*pit)->label() << " ";
			 out << endl;
		}
		
		 out << endl << "done" << endl;
		 out << "---------------------------" << endl;
	 }
	 
    // ----------------------------------------------------------------------
	 GeoObserver::MessageGeoTraceMapIt 
		GeoObserver::
		traces_begin() 
		throw()
	 {
		return traces_.begin();
	 }
      
    // ----------------------------------------------------------------------
	 GeoObserver::MessageGeoTraceMapIt 
		GeoObserver::
		traces_end() 
		throw()
	 {
		return traces_.end();
	 }
	 
    // ----------------------------------------------------------------------
	 NodeList 
		GeoObserver::
		shortest_path(shawn::Node& source, shawn::Node& dest) 
		throw(std::logic_error)
	 {
		if( ! distances_calculated_ )
		{
		  shawn::SimulationController& sc = source.world_w().simulation_controller_w();
		  shawn::NodeDistanceEstimateHandle nei = sc.distance_estimate_keeper_w().find_w("perfect_estimate");

		  if (nei.is_null())
			 throw new std::logic_error("Distance estimate not found");

		  calculate_distances(*(nei.get()) , sc);
		}
		
		NodeList nl;
		unsigned int sourceId = source.id();
		unsigned int destId = dest.id();

		if ( pred_[sourceId][destId] != UINT_INF)
		{
		  int j = destId;
		  while (pred_[sourceId][j] != sourceId) 
		  {
			 int tmp = pred_[sourceId][j];
			 nl.push_back(source.world_w().find_node_by_id_w(j));
			 j = tmp;
		  }
		  nl.push_back(source.world_w().find_node_by_id_w(j));
		  nl.push_back(source.world_w().find_node_by_id_w(sourceId));
		}

		return nl;
	 }	 
	 
    // ----------------------------------------------------------------------
    void 
      GeoObserver::
      geo_routing_send_to(shawn::Node& sender, shawn::MessageHandle& msg, const GeoRoutingAddress& addr)
      throw()
    {
      traces_[msg] = new GeoTrace(sender, addr, msg);
		traces_[msg]->visited(sender);
    }
      
    // ----------------------------------------------------------------------
    void 
      GeoObserver::
      geo_routing_process(shawn::Node& n, const GeoRoutingMessage&, GeoRoutingInfo& ri) 
      throw()
    {
      GeoTrace* trace = traces_[ri.message()];
      
      if( trace == NULL )
        cerr << "HMPF" << endl;
      else 
		  trace->visited(n);
		
    }
    
    // ----------------------------------------------------------------------
	 void 
		GeoObserver::
		calculate_distances( shawn::NodeDistanceEstimate& nde, shawn::SimulationController& sc ) 
		throw()
	 {
		cout << "Calculating distances" << endl;
		 shawn::World& w = sc.world_w();
		 
		 uint node_count = sc.world().node_count();
		 dist_.resize( node_count );
		 pred_.resize( node_count );
	 
		 // Initialization
		 for(uint i = 0; i < node_count; ++i) {
			 dist_[i].resize(node_count);
			 pred_[i].resize(node_count);
			 for(uint j = 0; j < node_count; ++j) {
				 pred_[i][j] = UINT_INF;
				 dist_[i][j] = (i == j) ? 0.0 : FLOAT_INF;
			 }
		 }
	 
		 //Import graph --> Create adjacency matrix
		 for( shawn::World::const_node_iterator it = w.begin_nodes(), end = w.end_nodes(); it != end; ++it)
		 {
			 const shawn::Node& nb = *it;
			 for(shawn::Node::const_adjacency_iterator nit = nb.begin_adjacent_nodes(), nend = nb.end_adjacent_nodes(); nit!=nend; ++nit)
			 {
				 double result;
				 nde.estimate_distance(nb, *nit, result );
				 dist_[nb.id()][nit->id()] = result; // TODO: Check is distance estimation was successful
				 pred_[nb.id()][nit->id()] = nb.id();
			 }
		 }
	 
		 //Algorithm main loop
		 for(uint  k = 0; k < node_count; ++k) {
			 for(uint  i = 0; i < node_count; ++i) {
				 for(uint  j = 0; j < node_count; ++j) {
					 if(dist_[i][k] == FLOAT_INF || dist_[k][j] == FLOAT_INF)
						 continue;
					 if(dist_[i][j] > dist_[i][k] + dist_[k][j]) {
						 dist_[i][j] = dist_[i][k] + dist_[k][j];
						 pred_[i][j] = pred_[k][j];
					 }
				 }
			 }
		 }

		cout << "Done calculating distances" << endl;
		 distances_calculated_ = true;
	 }
    
  }
}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/geo/test/geo_observer.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: geo_observer.cpp,v $
 *-----------------------------------------------------------------------*/


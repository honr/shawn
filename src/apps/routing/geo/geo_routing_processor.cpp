/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/geo/geo_routing_protocol.h"
#include "apps/routing/geo/geo_routing_processor.h"
#include "apps/routing/geo/geo_routing.h"
#include "sys/node.h"

#include <iostream>
using namespace std;

namespace routing 
{
    namespace geo
    {

        //-----------------------------------------------------------------------
        GeoRoutingProcessor::
            ~GeoRoutingProcessor()
        {
            GEO_DEBUG(5, "GeoRoutingProcessor::~GeoRoutingProcessor(): Instance destroyed");
        }
        
        //-----------------------------------------------------------------------
        void
            GeoRoutingProcessor::
            boot() 
            throw()
        {
			//Enable the feature to terminate the current node when all other (non-autoterminate)
			//processors are inactive.
			set_auto_terminate(true);

			//
            GEO_DEBUG(3, "GeoRoutingProcessor::boot[Node "<< owner().label() <<" ]: Initializing node"); 
            shawn::World& w = owner_w().world_w();
            
            //TODO: adapt the notification interval to a sensible value
            notification_interval_ = 10; 

            //Tell the scheduler to notify this node at a later point in time by calling timeout()
            GEO_DEBUG(3, "GeoRoutingProcessor::boot[Node "<< owner().label() <<" ]: Setting event @ " << notification_interval_); 
            event_handle_ = w.scheduler_w().new_event(*this, notification_interval_, NULL);            
        }

        //-----------------------------------------------------------------------
        void 
            GeoRoutingProcessor::
            timeout( shawn::EventScheduler&, shawn::EventScheduler::EventHandle, double curtime, shawn::EventScheduler::EventTagHandle& ) 
            throw ()
        {
            GEO_DEBUG(3, "GeoRoutingProcessor::Timeout[Node "<< owner().label() <<" ]: Current time: " << curtime); 
            double next_event_time = curtime + notification_interval_;
            shawn::World& w = owner_w().world_w();

            //TODO: Do something here (e.g. send messages, ...)
            
            GEO_DEBUG(3, "GeoRoutingProcessor::Timeout[Node "<< owner().label() <<" ]: Next event @ " << next_event_time); 
            w.scheduler_w().move_event(event_handle_, next_event_time);            
        }
        
        //-----------------------------------------------------------------------
        shawn::Node*
            GeoRoutingProcessor::
            next_hop(const GeoRoutingAddress& address) 
            throw()
        {
            shawn::Node& current = owner_w();
            const shawn::Vec& dest = address.destination();
            shawn::Vec mypos = node_pos(current);
            double shortest = (mypos - dest).euclidean_norm();
            shawn::Node* next = NULL;

			GEO_DEBUG(3, "GeoRoutingProcessor::next_hop: Node: "<< current.label() <<", pos: " << mypos << ", dist: " << shortest);

            for(shawn::Node::adjacency_iterator nit = current.begin_adjacent_nodes_w(), nend = current.end_adjacent_nodes_w(); nit!=nend; ++nit)
            {
                shawn::Node& nb = (*nit);

				if( nb == current )
					continue;

                shawn::Vec npos = node_pos(nb);
                double dist = (npos - dest).euclidean_norm();

                if( dist < shortest )
                {
                    GEO_DEBUG(3, "GeoRoutingProcessor::next_hop:cout << New next node: "<< nb.label() <<", pos: " << npos << ", dist: " << dist);
                    shortest = dist;
                    next = &nb;
                }
                else
                {
                    GEO_DEBUG(3, "GeoRoutingProcessor::next_hop:cout << Distance     : "<< nb.label() <<", pos: " << npos << ", dist: " << dist);
                }
            }

            if( next != NULL )
            {
                GEO_DEBUG(3, "GeoRoutingProcessor::next_hop:cout << Next node: "<< next->label() << ", dist: " << shortest);
            }

            return next; 
        }
        
        //-----------------------------------------------------------------------
        bool 
            GeoRoutingProcessor::
            process_message( const shawn::ConstMessageHandle& msg) 
            throw()
        {   
            GEO_DEBUG(3, "GeoRoutingProcessor::process_message requested");
            
            //Test if the received message is a GeoRoutingMessage
            const GeoRoutingMessage* rm = dynamic_cast<const GeoRoutingMessage*> (msg.get());
            if( rm != NULL ) 
            {
                //Retrieve the contained Routing information instance that is shared for all
                //messages of this routing process
                GeoRoutingInfo* ri = dynamic_cast<GeoRoutingInfo*> ( &rm->info() );
                if( ri != NULL )
                {
                    //Pass the received message to the routing process instance that is
                    //responsible for the handling of this special message instance.
                    //This is necessary, since possibly multiple different geo routing 
                    //instantiations may be concurrently active
                    GEO_DEBUG(4, "GeoRoutingProcessor::process_message: Passing message to the routing instance");
                    return ri->routing_instance().process_message(*this, owner_w(), *rm, *ri);
                }
            } 
            
            return false;
        }

        
        //-----------------------------------------------------------------------
        //-----------------------------------------------------------------------
        //-----------------------------------------------------------------------
        
        
        //-----------------------------------------------------------------------
        GeoRoutingProcessor::NeighborhoodMessage::
            NeighborhoodMessage(const shawn::Node& sender, NodeListIt begin, NodeListIt end) 
            throw()
            : sender_(sender)
        {
            neighbors_.insert(neighbors_.end(), begin, end);
        }
        //-----------------------------------------------------------------------
        const shawn::Node&
            GeoRoutingProcessor::NeighborhoodMessage::
            sender() 
            const throw()
        {
            return sender_;
        }
        //-----------------------------------------------------------------------
        GeoRoutingProcessor::NeighborhoodMessage::NodeListIt 
            GeoRoutingProcessor::NeighborhoodMessage::
            begin_neighbors() 
            throw()
        {
            return neighbors_.begin();
        }
        //-----------------------------------------------------------------------
        GeoRoutingProcessor::NeighborhoodMessage::NodeListIt 
            GeoRoutingProcessor::NeighborhoodMessage::
            end_neighbors() 
            throw()
        {
            return neighbors_.end();
        }
        //-----------------------------------------------------------------------
        unsigned int
            GeoRoutingProcessor::NeighborhoodMessage::
            neighbor_count() 
            const throw()
        {
            return neighbors_.size();

        }
        
        
        
    }
}
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/geo/geo_routing_processor.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: geo_routing_processor.cpp,v $
 *-----------------------------------------------------------------------*/

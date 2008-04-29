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

#include "sys/vec.h"
#include "sys/node.h"
#include "sys/world.h"
#include "sys/message.h"
#include "sys/misc/random/basic_random.h"

#include "apps/routing/geo/geo_routing.h"
#include "apps/routing/geo/geo_routing_protocol.h"
#include "apps/routing/geo/test/geo_test.h"
#include "apps/routing/geo/test/geo_postscript.h"

#include <assert.h>
#include <math.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;


namespace routing 
{
    
    namespace geo
    {
        GeoRoutingProtocol* GeoTestProcessor::routing_prot_ = NULL;

        ///Static variable initialization
        GeoObserver GeoTestProcessor::go;
        
        // ----------------------------------------------------------------------
        GeoTestProcessor::
            GeoTestProcessor()
        {}
            
        // ----------------------------------------------------------------------
        GeoTestProcessor::
            ~GeoTestProcessor()
        {}
            
        // ----------------------------------------------------------------------
        void
            GeoTestProcessor::
            routed_send(shawn::MessageHandle msg, shawn::Vec dest, shawn::Vec gran /* = shawn::Vec(10,10,0) */ ) 
            throw()
        {
            assert(routing_prot_ != NULL);
            msg->set_source( owner_w() );
            msg->set_timestamp( owner_w().simulation_round(), owner_w().current_time() );
            GeoRoutingAddress dest_addr(dest, gran);
            routing_prot_->send_to(owner_w(), msg, dest_addr);
        }
        
        // ----------------------------------------------------------------------
        void
            GeoTestProcessor::
            special_boot( void )
            throw()
        {
            GEO_DEBUG(1, "GeoTestProcessor::special_boot: Special boot of node " << owner().label()); 
            
            shawn::SimulationController& sc = owner_w().world_w().simulation_controller_w();
            send_interval_ = sc.environment().optional_double_param("send_interval", -1.0);
            
            //Get the configured routing protocol
            std::string routing_prot_name = sc.environment().optional_string_param("routing_protocol", "geo");
            RoutingProtocolBaseHandle rph = routing::routing_protocol_keeper_w(sc).find_w(routing_prot_name);
            assert(rph.is_not_null());
            
            routing_prot_ = dynamic_cast<GeoRoutingProtocol*> (rph.get());
            assert(routing_prot_ != NULL);
            
            GEO_DEBUG(1, "GeoTestProcessor::special_boot: Using routing protocol["<< routing_prot_name <<"]");

            routing_prot_->observer_add(go);

            if( send_interval_ > 0.0 )
            {
                GEO_DEBUG(1, "GeoTestProcessor::special_boot: Using send interval of " << send_interval_ );
                owner_w().world_w().scheduler_w().new_event(*this, send_interval_, NULL);            
            }
            
        }
        
        // ----------------------------------------------------------------------
        void 
            GeoTestProcessor::
            timeout( shawn::EventScheduler&, shawn::EventScheduler::EventHandle eh, 
                     double curtime, shawn::EventScheduler::EventTagHandle& )  
            throw ()
        {
            GEO_DEBUG(1, "GeoTestProcessor::timeout: Sending new message (time="<< curtime << ")");
            shawn::World& w = owner_w().world_w();
            shawn::Node* sender =   w.find_node_by_id_w( shawn::uniform_discrete_random(w.node_count()-1) );
            shawn::Vec ll = w.lower_left(), ur = w.upper_right();
            shawn::Vec dest_position = shawn::Vec(ll.x() + shawn::uniform_discrete_random( int(abs(long(ur.x() - ll.x() - 1.0)))), 
                                                  ll.y() + shawn::uniform_discrete_random( int(abs(long(ur.y() - ll.y() - 1.0)))), 
                                                  ll.z() + shawn::uniform_discrete_random( int(abs(long(ur.z() - ll.z() - 1.0)))) );
            assert( sender != NULL );
            GeoTestProcessor* sender_proc = sender->get_processor_of_type_w<GeoTestProcessor>();
            assert(sender_proc != NULL);

            GEO_DEBUG(1, "GeoTestProcessor::timeout: sender["<<(sender)<<"], sender_proc["<<sender_proc<<"], position["<< dest_position <<"]");
            
            sender_proc->routed_send( new GeoRoutingTestMessage, dest_position );
            
            
            w.scheduler_w().move_event(eh, curtime + send_interval_);            
        }
        
        // ----------------------------------------------------------------------
        bool
            GeoTestProcessor::
            process_message( const shawn::ConstMessageHandle& mh ) 
            throw()
        {
            const GeoRoutingTestMessage* hmsg = dynamic_cast<const GeoRoutingTestMessage*> ( mh.get() );
            if( hmsg == NULL )
                return false;
    
            GEO_DEBUG(1, "GeoTestProcessor::process_message: Node " << owner().label() << " received a GeoRoutingTestMessage");
            
            return Processor::process_message( mh );
        }
        
        // ----------------------------------------------------------------------
        void
            GeoTestProcessor::
            work( void )
            throw()
        {
            //set_state( Inactive );
        }

        // ----------------------------------------------------------------------
        void 
            GeoTestProcessor::
            run( shawn::SimulationController& sc )
            throw( std::runtime_error )
        {
            
            string psfile = sc.environment().required_string_param("psfile");
            bool shortest_path = sc.environment().optional_bool_param("draw_shortest_path", false);
            bool euclidean = sc.environment().optional_bool_param("draw_euclidean_dist", false);
            bool drawCR = sc.environment().optional_bool_param("draw_comm_range", false);
            bool draw_dest_area = sc.environment().optional_bool_param("draw_dest_area", true);
            double comm_range = sc.world().has_size_hint() ? sc.world().size_hint() : 0.0;
            bool need_new_page = false;
            GeoPostscript ps(sc, psfile);
            
            go.dump_paths(cout);
            GEO_DEBUG(1, "GeoTestProcessor::run: Options are: psfile["<< psfile <<"], draw_shortest_path["<< shortest_path <<"], " 
                        << "draw_euclidean_dist["<<  euclidean <<"], draw_comm_range["<< drawCR <<"], draw_dest_area["<< draw_dest_area <<"], "
                        << "comm_range["<< comm_range <<"]");
            
            for(GeoObserver::MessageGeoTraceMapIt it = go.traces_begin(), end = go.traces_end(); it!=end; ++it)
            {
                shawn::MessageHandle msg = it->first;
                GeoTrace& trace = *(it->second);
                std::list<shawn::Node*>::iterator sourcenode = trace.path_begin(), lastnode = trace.path_end();
                lastnode--; //end() is always pointing beyond the last entry

                //Start a new page if necessary
                if( need_new_page ) { ps.end_page(); ps.start_page(); }  else need_new_page = true;
                
                //Draw all nodes 
                ps.draw_nodes(sc.world().begin_nodes(), sc.world().end_nodes());

				//Draw destination area
                if(draw_dest_area)
                {
                    shawn::Vec p  = ps.pos2ps(trace.destination().destination());
                    shawn::Vec g = trace.destination().granularity() / 2;
                    shawn::Vec ur = shawn::Vec( p.x() - ps.scale(g.x()),p.y() - ps.scale(g.y()),p.z() - ps.scale(g.z()) );
                    shawn::Vec ll = shawn::Vec( p.x() + ps.scale(g.x()),p.y() + ps.scale(g.y()),p.z() + ps.scale(g.z()) );
                    
                    std::ostringstream oss;

                    oss << "0.95 setgray "
                        << ur.x() << " " << ur.y() << " moveto " 
                        << ll.x() << " " << ur.y() << " lineto " 
                        << ll.x() << " " << ll.y() << " lineto " 
                        << ur.x() << " " << ll.y() << " lineto fill " 
                        << p.x() << " " << p.y() << " destpos "                    
                        << endl;
                    
                    ps.custom( oss.str() );
                }
                
                //Shortest path
                if( shortest_path )
                {
                    shawn::Node* nearest_node = *lastnode;
                    double dist = (node_pos(*nearest_node) - trace.destination().destination()).euclidean_norm();
                    for(shawn::World::node_iterator wit = sc.world_w().begin_nodes_w(), wend = sc.world_w().end_nodes_w(); wit!= wend; ++wit)
                    {
                        double d = (node_pos(*wit) - trace.destination().destination()).euclidean_norm();
                        if( dist > d )
                        {
                            dist = d;
                            nearest_node = &(*wit);
                        }
                    }
                    assert(nearest_node != NULL);
                    
                    NodeList sp = go.shortest_path(**trace.path_begin(), *nearest_node);
                    ps.draw_path(sp.begin(), sp.end(), "spstyle");
                }

                //Message path
                ps.draw_path(trace.path_begin(), trace.path_end(), "msgpathstyle", drawCR, euclidean, trace.destination().destination(), comm_range);
                
                //Draw the source and the destination node
                {
                    shawn::Vec posbeg  = ps.pos2ps(node_pos(**sourcenode));
                    shawn::Vec posdest = ps.pos2ps(node_pos(**lastnode));
                    std::ostringstream oss;
                    
                    oss << posbeg.x() << " " << posbeg.y() << " sourcenode" << endl
                        << posdest.x() << " " << posdest.y() << " destnode" << endl;
                    
                    ps.custom( oss.str() );
                }

            }
            
            ps.end_page();
        }

        // ----------------------------------------------------------------------
        std::string 
            GeoTestProcessor::
            name( void ) 
            const throw()
        { 
            return "geo_test"; 
        }
        
        // ----------------------------------------------------------------------
        std::string 
            GeoTestProcessor::
            description( void ) 
            const throw()
        {
            return "Tests a geo-routing protocol. Options include: "
                    "psfile: The filename of the output postscript file"
                    "draw_shortest_path: (boolean, optional) "
                    "draw_euclidean_dist: (boolean, optional) "
                    "draw_comm_range: (boolean, optional) "
                    "draw_dest_area: (boolean, optional) ";
        }
        
    }
}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/geo/test/geo_test.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: geo_test.cpp,v $
 *-----------------------------------------------------------------------*/


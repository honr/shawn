/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#include "apps/routing/test/routing_test.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/routing_protocol_keeper.h"
#include "apps/routing/dummy/dummy_routing.h"
#include "apps/routing/flooding/flood_routing.h"
#include "apps/routing/geo/geo_routing_protocol.h"
#include "sys/misc/random/basic_random.h"
#include "sys/node.h"
#include "sys/message.h"

#include <iostream>
#include <fstream>
#include <set>
using namespace std;

namespace routing 
{
	// ----------------------------------------------------------------------
	RoutingTestProcessor::
		RoutingTestProcessor()
		: eval_round_(0),
		last_time_of_receive_( 0 ),
		ps_( NULL ),
		draw_topology_(true),
		verbose_(false)
	{}

	// ----------------------------------------------------------------------
	RoutingTestProcessor::
		~RoutingTestProcessor()
	{
		if( ps_ )
			delete ps_;
	}

	// ----------------------------------------------------------------------
	void
		RoutingTestProcessor::
		routed_send( shawn::MessageHandle msg ) 
		throw()
	{
		shawn::World& w = owner_w().world_w();
		shawn::Vec ll = w.lower_left(), ur = w.upper_right();

		routing::dummy::DummyRoutingProtocol* dummy_prot = dynamic_cast<routing::dummy::DummyRoutingProtocol*> (routing_prot_);
		routing::geo::GeoRoutingProtocol* geo_prot = dynamic_cast<routing::geo::GeoRoutingProtocol*> (routing_prot_);
		routing::flood::FloodRoutingProtocol* flood_prot = dynamic_cast<routing::flood::FloodRoutingProtocol*> (routing_prot_);

		msg->set_source( owner_w() );
		msg->set_timestamp( owner_w().simulation_round(), owner_w().current_time() );

		if( dummy_prot != NULL ) 
		{
			routing::dummy::DummyAddress da;
			dummy_prot->send_to(owner_w(), msg, da);
		} 
		else if( geo_prot != NULL )
		{
			shawn::Vec dest_position = shawn::Vec(
				ll.x() + shawn::uniform_discrete_random( int(abs(long(ur.x() - ll.x() - 1.0)))), 
				ll.y() + shawn::uniform_discrete_random( int(abs(long(ur.y() - ll.y() - 1.0)))), 
				ll.z() + shawn::uniform_discrete_random( int(abs(long(ur.z() - ll.z() - 1.0)))) );

			cerr << "Sending geo message to random position (" << dest_position.x() << "/" << dest_position.y() << ")" << endl;;
			geo_prot->send_to(owner_w(), msg, dest_position);
		}
		else if( flood_prot != NULL )
		{
			int hop_count = 10;

			//Use a hop count that may maximally span the whole region
			if( w.has_size_hint() )
			{
				cerr << "Got size hint: " << w.size_hint() << endl;
				hop_count = int(ur.x() - ll.x());
				hop_count = max(hop_count, int(ur.y() - ll.y()));
				hop_count = max(hop_count, int(ur.z() - ll.z()));
				hop_count /= int(w.size_hint());
			}

			hop_count = (hop_count/2) + shawn::uniform_discrete_random(hop_count/2);

			if( hop_count < 2 )
				hop_count = 2;

			cerr << "Sending message with hop count(" << hop_count << "), id("<< owner_w().id() <<")" << endl;
			flood_prot->send_to(owner_w(), msg, hop_count);
		}

	}

	// ----------------------------------------------------------------------
	void
		RoutingTestProcessor::
		boot( void )
		throw()
	{
		//Retrieve the configuration parameters for all nodes
		shawn::SimulationController& sc = owner_w().world_w().simulation_controller_w();
		eval_round_ = sc.environment().required_int_param("routing_test_eval_round");
		verbose_ = sc.environment().optional_bool_param("routing_test_verbose", false);
	}

	// ----------------------------------------------------------------------
	void
		RoutingTestProcessor::
		special_boot( void )
		throw()
	{
		//Retrieve the configuration parameters for the spcial node
		shawn::SimulationController& sc = owner_w().world_w().simulation_controller_w();
		string routing_prot_name = sc.environment().required_string_param("routing_test_protocol");
		string ps_file = sc.environment().optional_string_param("routing_test_psfile", "");
		draw_topology_ = sc.environment().optional_bool_param("routing_test_draw_topology", true);

		//Get the configured routing protocol
		RoutingProtocolBaseHandle rph = routing::routing_protocol_keeper_w(sc).find_w(routing_prot_name);
		routing_prot_ = rph.get();
		assert(routing_prot_ != NULL);

		//Initialize the postscript output
		if( ps_file != "" )
		{
			cerr << "RoutingTestProcessor: Creating postscript file " << ps_file << endl;
			ps_ = new RoutingTestPostscript(*(new ofstream(ps_file.c_str())));
			ps_->print_header(sc);
		}

		//Send one message (address will be set in the method)
		routed_send( new RoutingTestMessage( owner() ) );
	}

	// ----------------------------------------------------------------------
	bool
		RoutingTestProcessor::
		process_message( const shawn::ConstMessageHandle& mh ) 
		throw()
	{
		const RoutingTestMessage* hmsg = dynamic_cast<const RoutingTestMessage*> ( mh.get() );
		rx_this_round_.insert( mh );

		if( hmsg != NULL )
		{
			last_time_of_receive_=simulation_round();
			if(verbose_)
				cout << "I (node " << owner().id() << "), received routed message from node " << hmsg->source().id() << endl;
			return true;
		}

		return Processor::process_message( mh );
	}

	// ----------------------------------------------------------------------
	void
		RoutingTestProcessor::
		work( void )
		throw()
	{
		if( owner_w().is_special_node() )
		{
			set<const shawn::Node*> source_nodes, dest_nodes;
			shawn::World& w = owner_w().world_w();
			assert( ps_ != NULL );
			bool draw_page = false;

			for(shawn::World::node_iterator it = w.begin_nodes_w(), end = w.end_nodes_w(); it!=end; ++it)			
			{
				RoutingTestProcessor* p = (*it).get_processor_of_type_w<RoutingTestProcessor>();
				assert( p != NULL );
				if( p->rx_this_round_.size() > 0 )
				{
					draw_page = true;
					break;
				}
			}
			if( draw_page )
			{
				ps_->start_page();

				for(shawn::World::node_iterator it = w.begin_nodes_w(), end = w.end_nodes_w(); it!=end; ++it)
				{
					RoutingTestProcessor* p = (*it).get_processor_of_type_w<RoutingTestProcessor>();
					assert( p != NULL );

					for(std::set<shawn::ConstMessageHandle>::iterator mit = p->rx_this_round_.begin(), mend = p->rx_this_round_.end(); mit!=mend; ++mit)
					{
						const RoutingTestMessage* hmsg = dynamic_cast<const RoutingTestMessage*> ( (*mit).get() );

						if( hmsg != NULL )
						{
							assert( hmsg->has_source() );
							source_nodes.insert(&hmsg->source());
							dest_nodes.insert(&*it);
						}
						else // This is some other message -> assume its a routing message
						{
							ps_->draw_msg_path( *it, *mit );
						}
					}

					p->rx_this_round_.clear();
				}
				
				ps_->draw_nodes( owner().world().begin_nodes(),  owner().world().end_nodes(), draw_topology_);

				for(set<const shawn::Node*>::iterator it = source_nodes.begin(), end = source_nodes.end(); it!=end; ++it)
					ps_->draw_source_node(**it);
				for(set<const shawn::Node*>::iterator it = dest_nodes.begin(), end = dest_nodes.end(); it!=end; ++it)
					ps_->draw_dest_node(**it);

				ps_->end_page();
			}
		}

		if( owner_w().simulation_round() >= eval_round_ )
			set_state( shawn::Processor::Inactive );
	}

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/test/routing_test.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: routing_test.cpp,v $
*-----------------------------------------------------------------------*/

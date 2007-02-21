/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "routing_test.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/routing_protocol_keeper.h"
#include "apps/routing/dummy/dummy_routing.h"
#include "apps/routing/flooding/flood_routing.h"
#include "apps/routing/test/routing_test.h"
#include "sys/node.h"
#include "sys/message.h"

#include <iostream>
using namespace std;

namespace routing 
{
    RoutingTestProcessor::
        RoutingTestProcessor()
        : last_time_of_receive_( 0 )
    {}
    // ----------------------------------------------------------------------
    RoutingTestProcessor::
        ~RoutingTestProcessor()
    {}
    // ----------------------------------------------------------------------
    void
        RoutingTestProcessor::
        routed_send( shawn::MessageHandle msg ) 
        throw()
    {
        routing::dummy::DummyRoutingProtocol* dummy_prot = dynamic_cast<routing::dummy::DummyRoutingProtocol*> (routing_prot_);
        routing::flood::FloodRoutingProtocol* flood_prot = dynamic_cast<routing::flood::FloodRoutingProtocol*> (routing_prot_);

        msg->set_source( owner_w() );
        msg->set_timestamp( owner_w().simulation_round(), owner_w().current_time() );

        if( dummy_prot != NULL ) 
        {
            routing::dummy::DummyAddress da;
            dummy_prot->send_to(owner_w(), msg, da);
        } 
        else if( flood_prot != NULL )
        {
            int hop_count = 5;
            cerr << "Sending message with hop count(" << hop_count << "), id("<< owner_w().id() <<")" << endl;
            flood_prot->send_to(owner_w(), msg, hop_count);
        }

    }
    // ----------------------------------------------------------------------
    void
        RoutingTestProcessor::
        special_boot( void )
        throw()
    {
        //Get the configured routing protocol
        shawn::SimulationController& sc = owner_w().world_w().simulation_controller_w();
        std::string routing_prot_name = sc.environment().optional_string_param("routing_protocol", "dummy");
        RoutingProtocolBaseHandle rph = routing::routing_protocol_keeper_w(sc).find_w(routing_prot_name);
        routing_prot_ = rph.get();
        assert(routing_prot_ != NULL);

        //Send one message (no address set)
        routed_send( new RoutingTestMessage );
    }
    // ----------------------------------------------------------------------
    bool
        RoutingTestProcessor::
        process_message( const shawn::ConstMessageHandle& mh ) 
        throw()
    {
        const RoutingTestMessage* hmsg = dynamic_cast<const RoutingTestMessage*> ( mh.get() );
        if( hmsg != NULL )
        {
            last_time_of_receive_=simulation_round();
            neighbours_.insert( &hmsg->source() );
            cout << "ID '" << owner().label() << "' GOT HELLO FROM '" << hmsg->source().label() << "'" << endl;
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
//        if( simulation_round()-last_time_of_receive_ > 4 )
        if( simulation_round() == 10 )
        {
            cout << "ID " << id() << " DONE: " << (unsigned int)neighbours_.size() << " neighbours: ";

            for( std::set<const shawn::Node*>::const_iterator it = neighbours_.begin(), first = it, endit = neighbours_.end(); it != endit; ++it )
            {
                if( it != first )
                    cout << ", ";
                cout << "'"<<(**it).label()<<"'";
            }
            cout << endl;
            set_state( Inactive );
        }
    }

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/test/routing_test.cpp,v $
* Version $Revision: 1.1 $
* Date    $Date: 2005/10/11 15:24:22 $
*-----------------------------------------------------------------------
* $Log: routing_test.cpp,v $
 *-----------------------------------------------------------------------*/

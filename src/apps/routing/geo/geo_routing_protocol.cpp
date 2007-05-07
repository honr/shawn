/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/geo/geo_routing.h"
#include "apps/routing/geo/geo_routing_protocol.h"
#include "apps/routing/geo/geo_routing_processor.h"

#include "sys/node.h"

#include <iostream>
#include <limits>
using namespace std;

namespace routing 
{
    namespace geo
    {
        //-----------------------------------------------------------------------
        GeoRoutingProtocol::
            GeoRoutingProtocol(std::string name)
            : name_(name)
        {
            GEO_DEBUG(1, "GeoRoutingProtocol:: New instance name["<< name_ <<"]");
        }

        //-----------------------------------------------------------------------
        bool 
            GeoRoutingProtocol::
            is_receiver(shawn::Node& current, const GeoRoutingAddress& address) 
            const throw()
        {
            shawn::Vec mypos = node_pos(current);
            shawn::Vec gran = address.granularity() / 2;
            shawn::Box destarea = shawn::Box( mypos - gran , mypos + gran );
            
            if( destarea.contains( address.destination() ) )
            {
                GEO_DEBUG(1, "Node["<< current.label() <<"] is receiver for position["<< mypos <<"] with granularity/2["<< gran <<"]");
                return true;
            }
            
            return false;
        }
        
        //-----------------------------------------------------------------------
        void 
            GeoRoutingProtocol::
			observers_added(GeoRoutingObserver& obs) 
			throw()
		{
			GEO_DEBUG(1, "GeoRoutingProtocol::observer_added: Ignoring.");
		}

        //-----------------------------------------------------------------------
        void 
            GeoRoutingProtocol::
            send_to( shawn::Node& sender, shawn::MessageHandle& msg, const GeoRoutingAddress& address) 
            throw( std::runtime_error )
        {
            GEO_DEBUG(1, "GeoRoutingProtocol::send_to: Send requested: sender["<< sender.label() <<"]");

			//Inform the observers of this routing process
            for(ObserverIterator it = observers_begin(), end = observers_end(); it!=end; ++it)
                (**it).geo_routing_send_to(sender, msg, address);
 
			//Construct the envelope message that will contain the original message as well
			//as the address information
            GeoRoutingInfo* ri = new GeoRoutingInfo(*this, msg, sender, address, sender.current_time(), sender.simulation_round());
            GeoRoutingMessage* geomsg = new GeoRoutingMessage(ri);
            GeoRoutingProcessor* p = sender.get_processor_of_type_w<GeoRoutingProcessor>();
            assert(p != NULL);
            
            shawn::Node* next = p->next_hop(address);
            if( next != NULL )
                next->receive(geomsg);
            else
            { 
				GEO_DEBUG(1, "GeoRoutingProtocol::send_to: No next hop found, message dropped"); 
			}
        }

        //-----------------------------------------------------------------------
        bool
            GeoRoutingProtocol::
            process_message(GeoRoutingProcessor& p, shawn::Node& receiver, const GeoRoutingMessage& rmsg, GeoRoutingInfo& rinfo) 
            throw()
        {
            bool message_handled = false;
            
            GEO_DEBUG(2, "GeoRoutingProtocol::process_message: receiver["<< receiver.label() <<"]");
			
			//Notify the observers of this routing process
            for(ObserverIterator it = observers_begin(), end = observers_end(); it!=end; ++it)
                (**it).geo_routing_process(receiver, rmsg, rinfo);
            
			//Check if we are a receiver for this special message
			//If yes, pass the original message to the receiver of this routing message
			//If not, search a potential next hop
            if( is_receiver(receiver, rinfo.destination()) )
            {
                GEO_DEBUG(1, "GeoRoutingProtocol::process_message: Node is receiver["<< receiver.label() <<"]");
                receiver.receive( rinfo.message() );
                message_handled = true;
            }
            else
            {
                shawn::Node* next = p.next_hop(rinfo.destination());
                if( next != NULL )
                {
                    next->receive( new GeoRoutingMessage(&rinfo) );
                    message_handled = true;
                } 
                else
                {
                    GEO_DEBUG(1, "GeoRoutingProtocol::process_message: No next hop found, message dropped");
                }
            }
            return message_handled;
        }
        
        //-----------------------------------------------------------------------
        std::string 
            GeoRoutingProtocol::
            name() 
            const throw() 
        {
            return name_;
        }

        //-----------------------------------------------------------------------
        std::string 
            GeoRoutingProtocol::
            description() 
            const throw()
        {
            return "Geo routing protocol implementation";
        }
        
    }

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/geo/geo_routing_protocol.cpp,v $
* Version $Revision: 1.1 $
* Date    $Date: 2007/01/16 15:58:07 $
*-----------------------------------------------------------------------
* $Log: geo_routing_protocol.cpp,v $
 *-----------------------------------------------------------------------*/

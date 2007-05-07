/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "flood_routing.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/flooding/flood_routing.h"
#include "apps/routing/flooding/flood_routing_processor.h"
#include "sys/node.h"

namespace routing 
{
    namespace flood
    {

        //-----------------------------------------------------------------------
        FloodRoutingMessage::
            FloodRoutingMessage( shawn::MessageHandle& msg, const RoutedMessageInfoHandle& info, int hopsremain)
            : hopsremain_(hopsremain),
            info_(info)
        {}

        //-----------------------------------------------------------------------
        RoutedMessageInfo&
            FloodRoutingMessage::
            info() 
            const throw()
        {
            return *info_.get();
        }

        //-----------------------------------------------------------------------
        //-----------------------------------------------------------------------

        //-----------------------------------------------------------------------
        FloodRoutingProtocol::
            FloodRoutingProtocol(std::string name)
            : name_(name)
        {}

        //-----------------------------------------------------------------------
        void 
            FloodRoutingProtocol::
            send_to( shawn::Node& sender, shawn::MessageHandle& msg, const int& dist) 
            throw( std::runtime_error )
        {
            //Create a new FloodRoutingInfo instance that will be the same for this flooding procedure
            FloodRoutingInfo* fri = new FloodRoutingInfo(msg, sender, sender.current_time(), sender.simulation_round() );

            //Set the responsible flood routing instance. There may be multiple flood routing instances in the RoutingProtocolKeeper
            fri->flood_routing_instance_ = this;

            //Encapsulate the original message in a FloodRouting message that will be passed on to the next hop
            FloodRoutingMessage* frm = new FloodRoutingMessage(msg, fri, dist);
            frm->set_source(sender);
            frm->set_timestamp( sender.simulation_round(), sender.current_time() );

            //Process the message
            process_message(sender, *frm, *fri);
        }

        //-----------------------------------------------------------------------
        bool 
            FloodRoutingProtocol::
            process_message(shawn::Node& owner, const FloodRoutingMessage& frm, FloodRoutingInfo& fri)
            throw()
        {
            //Only continue if the message has not visited this node before
            if( fri.been_there_.find(&owner) != fri.been_there_.end() )
                return true;

            //Insert the node in the list of visited nodes
            fri.been_there_.insert(&owner);

            //Pass the packaged -original- message to this node
			if( fri.initial_sender() != owner )
				owner.receive( fri.message() );

            //Continue the flood wave if the hop limit is not reached
            if( frm.hopsremain_ > 0 )
			{
				routing::RoutedMessageInfo* rmi = &(frm.info());
				owner.send( new FloodRoutingMessage(fri.message(), rmi, frm.hopsremain_ - 1 ) );
			}

            return true;
        }

        //-----------------------------------------------------------------------
        std::string 
            FloodRoutingProtocol::
            name() 
            const throw() 
        {
            return name_;
        }

        //-----------------------------------------------------------------------
        std::string 
            FloodRoutingProtocol::
            description() 
            const throw()
        {
            return "Flood routing protocol";
        }

    }
}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/flooding/flood_routing.cpp,v $
* Version $Revision: 1.2 $
* Date    $Date: 2005/10/12 09:24:58 $
*-----------------------------------------------------------------------
* $Log: flood_routing.cpp,v $
 *-----------------------------------------------------------------------*/

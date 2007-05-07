/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_FLOOD_ROUTING_H
#define __SHAWN_APPS_ROUTING_FLOOD_ROUTING_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/routing.h"
#include <string>
#include <set>

namespace shawn 
{ 
    class Node; //Forward declaration
}

namespace routing 
{
    namespace flood 
    {
        class FloodRoutingProtocol;

        //-----------------------------------------------------------------------
        /** Routing message that comprises the original message as well as the
		  * remaining hops for the current flooding process. A handle to a 
		  * RoutedMessageInfo instance is kept as well. This is shared between all
		  * FloodRoutingMessages that participate in this flood wave. 
          */
        class FloodRoutingMessage : public routing::RoutingMessage
        {
        public:
			///Returns the single RoutedMessageInfo that is shared in this flood wave
            virtual RoutedMessageInfo& info() const throw();

        protected:
			/** Constructs a new routing message. Construction is limited to instances
			  * of FloodRoutingProcessor and FloodRoutingProtocol.
			  * @param msg The original message
			  * @param i The single routing instance
			  * @param h Remaining hops
			  */
            FloodRoutingMessage( shawn::MessageHandle& msg, const RoutedMessageInfoHandle& i, int h);

			///The remaining hops
            int hopsremain_;

			///Handle to the RoutedMessage info as described above.
            RoutedMessageInfoHandle info_;

            friend class FloodRoutingProcessor;
            friend class FloodRoutingProtocol;
        };

        //-----------------------------------------------------------------------
        /** Instances of this class are shared for all FloodRoutingMessage messages of
		  * one flood wave. It stores the shawn::Node instances that have been visited
		  * by this flood wave. In doing so, each node will receive and forward the message
		  * only once, thus limiting the broadcast storm. It may only be used by instances 
		  * of FloodRoutingProcessor and FloodRoutingProtocol because all members are at least
		  * protected.
          */
        class FloodRoutingInfo : public routing::RoutedMessageInfo
        {
        protected:
            FloodRoutingInfo(shawn::MessageHandle& msg, shawn::Node& sender, double time, int round) 
                : routing::RoutedMessageInfo(msg, sender, time, round)
            {}

        private:
			///The nodes that have already received this flood wave
            std::set<shawn::Node*> been_there_;

			///The instance that has triggered the wave. 
            FloodRoutingProtocol* flood_routing_instance_;

            friend class FloodRoutingProcessor;
            friend class FloodRoutingProtocol;
        };

        //-----------------------------------------------------------------------
        /** The actual controlled flooding routing protocol implementation. 
		  * The destination address is the number of hops count of the flood wave.
		  * Each visited node will be cached and hence will only receive the message
		  * once.
		  * 
		  * This routing protocol must be incarnated before any Node is added to the World. This is
		  * because this routing protocol adds a default processor to the list of default processors
		  * in the World and hence these can only be attached when nodes are created.
          */
        class FloodRoutingProtocol : public RoutingProtocol<int> 
        {
		protected:
			///Instantiated by the friend class FloodRoutingProtocolFactory
            FloodRoutingProtocol(std::string name);

        public:
			/** 
			  * @param sender The sending node that initiates this flood wave
			  * @param msg The message to transmit
			  * @param hop_count Maximum number of hops
			  */ 
            virtual void send_to( shawn::Node& sender, shawn::MessageHandle& msg, const int& hop_count) 
                throw( std::runtime_error );

			///Returns the name of this flood routing instance as created by routing::RoutingProtocolTask
            std::string name() const throw();
            std::string description() const throw();

			/** Process message is redirected from the FloodRoutingProcessor to this instance of the flood routing.
			  **/
            virtual bool process_message(shawn::Node&, const FloodRoutingMessage&, FloodRoutingInfo& ) throw();

        private:
			///The name of this protocol instance. 
            std::string name_;
            friend class FloodRoutingProtocolFactory;
        };
    }
}

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/flooding/flood_routing.h,v $
* Version $Revision: 1.1 $
* Date    $Date: 2005/10/11 15:24:21 $
*-----------------------------------------------------------------------
* $Log: flood_routing.h,v $
 *-----------------------------------------------------------------------*/

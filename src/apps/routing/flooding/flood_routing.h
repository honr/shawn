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
    class Node; 
}

namespace routing 
{
    namespace flood 
    {
        class FloodRoutingProtocol;

        /**
          *
          */
        class FloodRoutingMessage : public routing::RoutingMessage
        {
        public:

            virtual RoutedMessageInfo& info() const throw();

        protected:
            FloodRoutingMessage( shawn::MessageHandle& msg, const RoutedMessageInfoHandle&, int );

            int hopsremain_;
            RoutedMessageInfoHandle info_;

            friend class FloodRoutingProcessor;
            friend class FloodRoutingProtocol;
        };

        /**
          *
          */
        class FloodRoutingInfo : public routing::RoutedMessageInfo
        {
        protected:
            FloodRoutingInfo(shawn::MessageHandle& msg, shawn::Node& sender, double time, int round) 
                : routing::RoutedMessageInfo(msg, sender, time, round)
            {}

        private:
            std::set<shawn::Node*> been_there_;
            FloodRoutingProtocol* flood_routing_instance_;

            friend class FloodRoutingProcessor;
            friend class FloodRoutingProtocol;
        };

        /**
          *
          */
        class FloodRoutingProtocol : public RoutingProtocol<int> 
        {
            FloodRoutingProtocol(std::string name);

        public:
            virtual void send_to( shawn::Node& sender, shawn::MessageHandle&, const int& ) 
                throw( std::runtime_error );

            std::string name() const throw();
            std::string description() const throw();

            virtual bool process_message(shawn::Node&, const FloodRoutingMessage&, FloodRoutingInfo& ) throw();

        private:
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

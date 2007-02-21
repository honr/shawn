/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_ROUTING_GEO_ROUTING_PROCESSOR_H
#define __SHAWN_APPS_ROUTING_GEO_ROUTING_PROCESSOR_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "sys/message.h"
#include "sys/processor.h"
#include "sys/world.h"
#include "sys/node.h"
#include "sys/event_scheduler.h"

#include "apps/routing/geo/geo_routing.h"

#include "apps/routing/routing.h"

#include <string>
#include <list>


namespace routing 
{
    namespace geo
    {
      
        //-------------------------------------------------------------------------------------------
        /** This is a helper class for the GeoRoutingProtocol class. This shawn::Processor is attached to
          * each node of the world implicitely by using the shawn::World::add_default_processor() mechanism.
          * The processor will get all messages that are received by a node. A processor can also send messages
          * using the inherited Processor::send method. This class also implements the shawn::EventScheduler::EventHandler
          * interface in order to be able to be notified at certain point in time. This can be used to regularly send
          * messages to neighbors or similar actions.
          */
        class GeoRoutingProcessor : public shawn::Processor, public shawn::EventScheduler::EventHandler
        {
        protected:
          
          //-------------------------------------------------------------------------------------------
          /** Internal use (protected class). Used to exchange neighborhood information data. 
            * You may use the node pointers to obtain the remote position using routing::node_position().
            */
          class NeighborhoodMessage : public shawn::Message {
            public:
              ///Used to exchange neighborhood lists
              typedef std::list<const shawn::Node*> NodeList;
              ///Used to iterate neighborhood lists
              typedef NodeList::iterator NodeListIt;
            
              /// Construct a new neighborhood information package
              NeighborhoodMessage(const shawn::Node&, NodeListIt, NodeListIt) throw();
              /// Returns the sender of the message
              const shawn::Node& sender() const throw();
              ///
              NodeListIt begin_neighbors() throw();
              ///
              NodeListIt end_neighbors() throw();
              ///
              unsigned int neighbor_count() const throw();
            
            protected:
              NodeList neighbors_;
              const shawn::Node& sender_;
          };
          
        public:
            
            /** Destructor. Be aware, that the world instance may not be available anymore.
              */
            virtual ~GeoRoutingProcessor();
        
            /** This method is called once on startup of the shawn::World. Use this to initialize 
              * your application instead of the constructor. This method is called for every instance
              * of this class. Usually this is the number of nodes in this world.
              */
            virtual void boot() throw();

            /** Called whenever the owner node receives a message. 
              * Return true when the message has been fully processed and no other processor
              * should receive the message. Return false when the message should be passed
              * to other processors (should be the default).
              */
            virtual bool process_message( const shawn::ConstMessageHandle& ) throw();

            /**  This method is called by the EventScheduler when the timeout of a registered Event is reached
              * This is used to inform subscribers when a Box has been reached
              */
            virtual void timeout( shawn::EventScheduler&, 
                                  shawn::EventScheduler::EventHandle, 
                                  double, 
                                  shawn::EventScheduler::EventTagHandle& ) throw ();
            
            /** Determines the next hop to use by looking at locally stored data. 
              */
            virtual shawn::Node* next_hop(const GeoRoutingAddress& address) throw();
            
        protected:
            ///The current event 
            shawn::EventScheduler::EventHandle event_handle_;
            ///The interval for which events are set
            double notification_interval_;
            
        };
    }
}

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/geo/geo_routing_processor.h,v $
* Version $Revision: 1.1 $
* Date    $Date: 2007/01/16 15:58:07 $
*-----------------------------------------------------------------------
* $Log: geo_routing_processor.h,v $
 *-----------------------------------------------------------------------*/

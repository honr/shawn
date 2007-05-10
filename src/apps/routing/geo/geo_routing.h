/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_GEO_ROUTING_H
#define __SHAWN_APPS_ROUTING_GEO_ROUTING_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/routing.h"
#include "sys/vec.h"
#include "sys/node.h"
#include <ostream>
#include <string>
#include <set>

#define GEO_DEBUGLEVEL 1
#ifdef GEO_DEBUGLEVEL 
#define GEO_DEBUG(level, x) if ((level) <= (GEO_DEBUGLEVEL)) cout << "GeoRoutingProcessor: " << x << endl;
#else
#define GEO_DEBUG(level, x)
#endif

namespace routing 
{
    namespace geo 
    {
        //Forward declaration
        class GeoRoutingProtocol;

        //-------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------
        /** Small helper method to determine the position of a node. If an estimated position is
          * available, it is returned. Otherwise, the real position is used.
          */
        inline const shawn::Vec node_pos(const shawn::Node& n) 
        {
            return n.has_est_position() ? n.est_position() : n.real_position();
        }
            
        //-------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------
        /** Specifies an address in terms of coordinates and a granularity vector.
            If a node is within the area ( (destination_ - granularity/2), (destination_ + granularity/2) )
            then it is considered a valid recipient of for this address
        */
        class GeoRoutingAddress
        {
            shawn::Vec destination_; ///< The destination address
            shawn::Vec granularity_; ///< Specifies the area in which the destination is valid

        public:       
            ///Constructs a new GeoRouting Address
            GeoRoutingAddress(shawn::Vec destination, shawn::Vec granularity = shawn::Vec(10,10,10))
                : destination_(destination), granularity_(granularity)
            {}
            
            ///Returns the destination address
            const shawn::Vec& destination() const { return destination_; }
            ///Returns the area in which the destination is valid
            const shawn::Vec& granularity() const { return granularity_; }
        };
        
        //-------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------
        /** A routing-message class used internally by the GeoRoutingProtocol to exchange messages.
          * Instantiation is only allowed for the friend classes.
          */
        class GeoRoutingMessage : public routing::RoutingMessage
        {
        public:
            
            /** Returns a reference to the RoutedMessageInfo instance that is shared for all 
              * GeoRoutingMessage instances of one routing task.
              */
            virtual RoutedMessageInfo& info() const throw();

        protected:
            /** Constructs a new GeoRoutingMessage. As parameter, a RoutedMessageInfoHandle ist
              * requested, but you can pass a (RoutedMessageInfo*), since an implicit conversion
              * exists. 
              */
            GeoRoutingMessage(const RoutedMessageInfoHandle&);
        
            ///Reference to the RoutedMessageInfo instance
            RoutedMessageInfoHandle info_;        
            friend class GeoRoutingProtocol;
            friend std::ostream& operator<<(std::ostream& os, const GeoRoutingMessage& m);
        };
        
        ///Output operator for GeoRoutingMessage classes
        std::ostream& operator<<(std::ostream&, const GeoRoutingMessage&);
        
        //-------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------
        /** This class encapsulates information that is used to store information about the routing process.
          * Each individual message routing will use exactly one instance of this class and shares it with
          * all GeoRoutingMessage instances which cooperate for one routing process.
          */
        class GeoRoutingInfo : public routing::RoutedMessageInfo
        {
        protected:
            ///
            GeoRoutingInfo(GeoRoutingProtocol& routing_instance, shawn::MessageHandle& msg, shawn::Node& sender, GeoRoutingAddress dest, double time, int round) 
                : routing::RoutedMessageInfo(msg, sender, time, round), 
                  routing_instance_(routing_instance),
                  dest_(dest)
            {}

            ///
            GeoRoutingProtocol& routing_instance() const { return routing_instance_; }

        public:            
            ///
            const GeoRoutingAddress& destination() const { return dest_; } 
                
        private:
            ///
            GeoRoutingProtocol& routing_instance_;
            ///
            GeoRoutingAddress dest_;
        
            friend class GeoRoutingProcessor;
            friend class GeoRoutingProtocol;
        };
        
        //-------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------
        /** A geo routing observer is used to monitor the process of the routing. Observers implementing this
          * interface can register themselves with the GeoRoutingProtocol class, since it inherits from
          * Observable<GeoRoutingObserver>
          */
        class GeoRoutingObserver 
        {
            public:
                ///Callback method.
                virtual void geo_routing_send_to(shawn::Node& sender, shawn::MessageHandle&, const GeoRoutingAddress&) throw() = 0;
                ///Callback method.
                virtual void geo_routing_process(shawn::Node&, const GeoRoutingMessage&, GeoRoutingInfo& ) throw() = 0;
        };

      
    }
}

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/geo/geo_routing.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: geo_routing.h,v $
 *-----------------------------------------------------------------------*/

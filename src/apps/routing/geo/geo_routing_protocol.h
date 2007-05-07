/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_GEO_ROUTING_PROTOCOL_H
#define __SHAWN_APPS_ROUTING_GEO_ROUTING_PROTOCOL_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/geo/geo_routing.h"
#include "apps/routing/routing.h"
#include "sys/misc/observable.h"
#include "sys/vec.h"
#include "sys/node.h"
#include <string>
#include <set>

namespace routing 
{
    namespace geo 
    {

        //-------------------------------------------------------------------------------------------
        /** The main class of the geo routing protocol implementation. This class will be instantiated using
          * the GeoRoutingProtocolFactory class, since direct instantiation is not allowed. To use this routing
          * protocol in shawn, use the "create_routing_protocol" task in the shawn configfile (e.g. 
          * create_routing_protocol name=my_app_routing protocol=geo). This will create a new routing instance 
          * named "my_app_routing" of the type "geo". To use this newly created protocol, you have to retrieve
          * it in your code via the RoutingProtocolKeeper. An example code in a shawn::Processor could be 
          * <pre>
          *      shawn::SimulationController& sc = owner_w().world_w().simulation_controller_w();
          *      std::string routing_prot_name = sc.environment().optional_string_param("my_app_routing", "geo");
          *      RoutingProtocolBaseHandle rph = routing::routing_protocol_keeper_w(sc).find_w(routing_prot_name);
          *      assert(rph.is_not_null());
          *      GeoRoutingProtocol* routing_prot_ = dynamic_cast<GeoRoutingProtocol*> (rph.get());
          *      assert(routing_prot_ != NULL);
          * </pre>
          */
        class GeoRoutingProtocol 
			: public routing::RoutingProtocol<GeoRoutingAddress>, 
              public shawn::Observable<GeoRoutingObserver>
        {
        protected:
            //---------------------------------------------------------------------
            ///@name construction / destruction / factory
            ///@{        
            
                /** Protected construction. See GeoRoutingProtocolFactory for information on how to create instances 
                    of this class. 
                    @param name The supplied name will be used as the return value for name().
                */
                GeoRoutingProtocol(std::string name);
            
            ///@}

            //---------------------------------------------------------------------
            ///@name Main (internal) routing logic
            ///@{        
                /** This is a callback method that will be invoked by the GeoRoutingProcessor whenever a node
                  * receives a message that is of type GeoRoutingMessage. The GeoRoutingProcessor will hand
                  * this to the corresponding GeoRoutingProtocol instance that has initiated the routing
                  * process.
                  * @param p The processor instance that received the message
                  * @param receiver The node to which the processor belongs (convenience for p.owner_w())
                  * @param rmsg The routing message that was received
                  * @param rinfo A reference to the single GeoRoutingInfo instance that belongs to one routing task
                  * @return true If the message has been processed and no other shawn::Processor should receive it, false otherwise
                  */
                virtual bool process_message(GeoRoutingProcessor& p, shawn::Node& receiver, const GeoRoutingMessage& rmsg, GeoRoutingInfo& rinfo) throw();
                
                ///Checks whether the current receiver node is a receiver for the address
                bool is_receiver(shawn::Node& current, const GeoRoutingAddress& address) const throw();

            ///@}
                        
        public:
			///To satisfy the Observable interface
			virtual void observers_added(GeoRoutingObserver& obs) throw();

            //---------------------------------------------------------------------
            ///@name Routing interface
            ///@{        
        
                /** Implements the routing protocol interface for the nodes/processors. They will pass the message to be sent  
                  * and the destination address to this method and hope for delivery 
                  */
                virtual void send_to( shawn::Node& sender, shawn::MessageHandle&, const GeoRoutingAddress& ) 
                    throw( std::runtime_error );
    
                ///Returns the name of the routing protocol instance
                std::string name() const throw();
            
                ///A short (human readable) description of this protocol
                std::string description() const throw();

            ///@}

        private:
            ///The name of this routing protocol instance
            std::string name_;
            friend class GeoRoutingProtocolFactory;
            friend class GeoRoutingProcessor;
        };
    }
}

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/geo/geo_routing_protocol.h,v $
* Version $Revision: 1.1 $
* Date    $Date: 2007/01/16 15:58:07 $
*-----------------------------------------------------------------------
* $Log: geo_routing_protocol.h,v $
 *-----------------------------------------------------------------------*/

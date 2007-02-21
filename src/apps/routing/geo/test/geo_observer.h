/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_ROUTING_GEO_TEST_GEO_OBSERVER_H
#define __SHAWN_APPS_ROUTING_GEO_TEST_GEO_OBSERVER_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/geo/geo_routing.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/node_distance_estimate.h"
#include "sys/node.h"
#include "sys/message.h"

#include <deque>
#include <vector>
#include <iostream>


namespace routing 
{
    namespace geo
    {
      typedef std::list<shawn::Node*> NodeList;
      typedef NodeList::iterator NodeListIt;
    
        class GeoTrace
        {          
          public: 

            GeoTrace(shawn::Node& sender, GeoRoutingAddress dest, shawn::MessageHandle);
            
            NodeListIt path_begin();
            NodeListIt path_end();
            shawn::Node& sender();
            const GeoRoutingAddress& destination();
            shawn::MessageHandle message();
            
            void visited(shawn::Node& node);
          
          protected:
            shawn::Node& sender_;
            GeoRoutingAddress dest_; 
            shawn::MessageHandle msg_;
            NodeList path_;
        };
      
      
        /**
        *
        */
        class GeoObserver : public GeoRoutingObserver
        {
          typedef unsigned int uint;
          typedef std::vector< std::vector<double> > FloatMatrix;
          typedef std::vector< std::vector<uint> > UINTMatrix;
          
          //
          void calculate_distances( shawn::NodeDistanceEstimate& nde, shawn::SimulationController& sc ) throw();
          
        public:
            //---------------------------------------------------------------------
            ///@name Type definitions
            ///@{        
                typedef std::map<shawn::MessageHandle, GeoTrace*> MessageGeoTraceMap;
                typedef MessageGeoTraceMap::iterator MessageGeoTraceMapIt;
            ///@}
        
            //---------------------------------------------------------------------
            ///@name GeoObserver
            ///@{        
                GeoObserver();
                virtual ~GeoObserver();
                void dump_paths(std::ostream& out);
        
                NodeList shortest_path(shawn::Node& source, shawn::Node& dest) throw(std::logic_error);
                MessageGeoTraceMapIt traces_begin() throw();
                MessageGeoTraceMapIt traces_end() throw();
            ///@}
    
            //---------------------------------------------------------------------
            ///@name GeoRoutingObserver interface
            ///@{        
                virtual void geo_routing_send_to(shawn::Node& sender, shawn::MessageHandle&, const GeoRoutingAddress&) throw();
                virtual void geo_routing_process(shawn::Node&, const GeoRoutingMessage&, GeoRoutingInfo& ) throw();
            ///@}
    
        
        private:
            bool distances_calculated_;
            FloatMatrix dist_;
            UINTMatrix pred_;
            MessageGeoTraceMap traces_;
        };
        
    }
}

#endif
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/geo/test/geo_observer.h,v $
* Version $Revision: 1.1 $
* Date    $Date: 2007/01/16 15:58:07 $
*-----------------------------------------------------------------------
* $Log: geo_observer.h,v $
 *-----------------------------------------------------------------------*/


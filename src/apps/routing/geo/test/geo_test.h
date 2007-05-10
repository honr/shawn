/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_ROUTING_GEO_TEST_GEO_TEST_H
#define __SHAWN_APPS_ROUTING_GEO_TEST_GEO_TEST_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/geo/geo_routing.h"
#include "apps/routing/geo/test/geo_observer.h"

#include "sys/event_scheduler.h"
#include "sys/processors/processor_factory.h"
#include "sys/processor.h"
#include "sys/processors/processor_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/message.h"

#include <set>

namespace routing 
{
    namespace geo
    {
        // ----------------------------------------------------------------------
        // ----------------------------------------------------------------------
        /**
        *
        */
        class GeoRoutingTestMessage
            : public shawn::Message
        {
        public:
            GeoRoutingTestMessage() {}
            virtual ~GeoRoutingTestMessage() {}
        };
        
        
        // ----------------------------------------------------------------------
        // ----------------------------------------------------------------------
        /**
        * In this class, the name/description methods are identical for ProcessorFactory and SimulationTask
        */
        class GeoTestProcessor 
            : public shawn::Processor, 
              public shawn::SimulationTask,
              public shawn::EventScheduler::EventHandler
        {
        protected:
            //Construction via the Factory
            GeoTestProcessor();
        
        public:
            //---------------------------------------------------------------------
            ///@name GeoTestProcessor
            ///@{        
                virtual ~GeoTestProcessor();
                virtual void routed_send(shawn::MessageHandle msg, shawn::Vec dest, shawn::Vec gran = shawn::Vec(10,10,0)) throw();
            ///@}
        
            //---------------------------------------------------------------------
            ///@name shawn::Processor interface
            ///@{        
                virtual void special_boot( void ) throw();
                virtual bool process_message( const shawn::ConstMessageHandle& ) throw();
                virtual void work( void ) throw();
            ///@}
        
            //---------------------------------------------------------------------
            ///@name shawn::Task interface
            ///@{        
                ///
                virtual void run( shawn::SimulationController& sc ) throw( std::runtime_error );
                ///
                virtual std::string name( void ) const throw();
                ///
                virtual std::string description( void ) const throw();
            ///@}
    
            //---------------------------------------------------------------------
            ///@name shawn::EventScheduler::EventHandler interface
            ///@{        
                ///This method is called by the EventScheduler when the timeout of a registered Event is reached
                virtual void timeout( shawn::EventScheduler&, 
                                      shawn::EventScheduler::EventHandle, 
                                      double, 
                                      shawn::EventScheduler::EventTagHandle& )  throw ();
            ///@}
    
            //---------------------------------------------------------------------
            
            //Processor Factory implementation
            /**
              *
              */
            class Factory : public shawn::ProcessorFactory
            {
                public: 
                    virtual std::string name( void ) const throw()
                    { 
                        return "geo_test"; 
                    }
                    virtual std::string description( void ) const throw()
                    {
                        return "Tests geo-routing protocol"; 
                    }
                    virtual shawn::Processor* create( void ) throw()
                    { 
                        return new GeoTestProcessor; 
                    }
                    static void register_factory( shawn::SimulationController& sc ) throw()
                    { 
                        sc.processor_keeper_w().add( new Factory ); 
                    }
                    static void register_task( shawn::SimulationController& sc ) throw()
                    { 
						sc.simulation_task_keeper_w().add( new routing::geo::GeoTestProcessor );
                    }
            };

            friend class GeoTestProcessor::Factory;
                
        private:
            ///The sending interval of the special instance
            double send_interval_;
            ///The routing protocol instance that we use to send our messages
            static GeoRoutingProtocol* routing_prot_;
            ///The single Observer instance
            static GeoObserver go;
        };
        
    }
}

#endif
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/geo/test/geo_test.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: geo_test.h,v $
 *-----------------------------------------------------------------------*/


/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_TEST_ROUTING_TEST_H
#define __SHAWN_APPS_ROUTING_TEST_ROUTING_TEST_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/dummy/dummy_routing.h"
#include "apps/routing/flooding/flood_routing.h"
#include "apps/routing/test/routing_test_postscript.h"
#include "sys/processors/processor_factory.h"
#include "sys/processor.h"
#include "sys/processors/processor_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/message.h"
#include <set>
#include <string>

namespace routing 
{
    /**
    *
    */
    class RoutingTestMessage
        : public shawn::Message
    {
    public:
		const shawn::Node& source_;
		RoutingTestMessage(const shawn::Node& source) : source_(source) {}
        virtual ~RoutingTestMessage() {}
    };

    /**
    *
    */
    class RoutingTestProcessor 
		: public shawn::Processor, 
		  public shawn::ProcessorFactory
    {
    public:
        RoutingTestProcessor();
        virtual ~RoutingTestProcessor();
        virtual void routed_send( shawn::MessageHandle ) throw();

        //Processor
        virtual void boot( void ) throw();
        virtual void special_boot( void ) throw();
        virtual bool process_message( const shawn::ConstMessageHandle& ) throw();
        virtual void work( void ) throw();

        //Processor Factory implementation
        virtual std::string name( void ) const throw()
        { 
            return "routing_test"; 
        }
        virtual std::string description( void ) const throw()
        {
            return "Tests routing protocols"; 
        }
        virtual shawn::Processor* create( void ) throw()
        { 
            return new RoutingTestProcessor; 
        }
        static void register_factory( shawn::SimulationController& sc ) throw()
        { 
            sc.processor_keeper_w().add( new RoutingTestProcessor); 
        }

    private:
        int eval_round_;
        int last_time_of_receive_;
        RoutingProtocolBase* routing_prot_;
		RoutingTestPostscript* ps_;
		std::set<shawn::ConstMessageHandle> rx_this_round_;
		bool draw_topology_;
		bool verbose_;
    };

}

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/test/routing_test.h,v $
* Version $Revision: 1.1 $
* Date    $Date: 2005/10/11 15:24:22 $
*-----------------------------------------------------------------------
* $Log: routing_test.h,v $
 *-----------------------------------------------------------------------*/

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_ROUTING_DUMMY_ROUTING_H
#define __SHAWN_APPS_ROUTING_DUMMY_ROUTING_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/routing.h"
#include <string>

namespace routing 
{
    namespace dummy 
    {

        class DummyAddress
        {};

        class DummyRoutingProtocol : public routing::RoutingProtocol<DummyAddress>
        {
            DummyRoutingProtocol(std::string name);
        public:

            virtual void send_to( shawn::Node& sender, shawn::MessageHandle&, const DummyAddress& ) 
                throw( std::runtime_error );

            std::string name() const throw();
            std::string description() const throw();

        private:
            friend class DummyRoutingProtocolFactory;
            std::string name_;
        };
    }
}

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/dummy/dummy_routing.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: dummy_routing.h,v $
 *-----------------------------------------------------------------------*/

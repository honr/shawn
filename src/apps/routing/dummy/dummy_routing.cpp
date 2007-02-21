/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "dummy_routing.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/dummy/dummy_routing.h"
#include "sys/node.h"

namespace routing 
{
    namespace dummy
    {
        //-----------------------------------------------------------------------
        DummyRoutingProtocol::
            DummyRoutingProtocol(std::string name)
            : name_(name)
        {}

        //-----------------------------------------------------------------------
        void 
            DummyRoutingProtocol::
            send_to( shawn::Node& sender, shawn::MessageHandle& msg, const DummyAddress& ) 
            throw( std::runtime_error )
        {
            sender.send( msg );
        }

        //-----------------------------------------------------------------------
        std::string 
            DummyRoutingProtocol::
            name() 
            const throw() 
        {
            return name_;
        }

        //-----------------------------------------------------------------------
        std::string 
            DummyRoutingProtocol::
            description() 
            const throw()
        {
            return "A dummy routing protocol, only calls Node::send() to broadcast the"
                " message to all neighbors";
        }

    }
}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/dummy/dummy_routing.cpp,v $
* Version $Revision: 1.1 $
* Date    $Date: 2005/10/11 15:24:21 $
*-----------------------------------------------------------------------
* $Log: dummy_routing.cpp,v $
 *-----------------------------------------------------------------------*/

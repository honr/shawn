/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_ROUTING_ROUTING_H
#define __SHAWN_SYS_ROUTING_ROUTING_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include <stdexcept>

#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/refcnt_pointable.h"

#include "sys/message.h"
#include "apps/routing/routed_message_info.h"

namespace shawn 
{
    class Node;
}

namespace routing
{

    /**
    *
    */
    class RoutingMessage 
        : public shawn::Message
    {
    public:
        virtual RoutedMessageInfo& info() const throw() = 0;
    };

    /**
    *
    */
    class RoutingProtocolBase 
        : public shawn::KeeperManaged
    {};
    DECLARE_HANDLES(RoutingProtocolBase);

    /**
    *
    */
    template<typename AbstractAddress>
    class RoutingProtocol
        : public RoutingProtocolBase
    {
    public:

        virtual void send_to( shawn::Node& sender, shawn::MessageHandle&, const AbstractAddress& ) 
            throw( std::runtime_error ) = 0;

    };

}

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/routing.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: routing.h,v $
 *-----------------------------------------------------------------------*/

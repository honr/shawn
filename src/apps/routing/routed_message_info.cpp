/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "apps/routing/routed_message_info.h"

#ifdef ENABLE_ROUTING

using namespace shawn;

namespace routing 
{
    long RoutedMessageInfo::next_id_ = 0;

    // --------------------------------------------------------------------
    RoutedMessageInfo::
        RoutedMessageInfo(shawn::MessageHandle& msg, shawn::Node& sender, double time, int round)
        : msg_(msg), 
          sender_(&sender), 
          time_(time), 
          round_(round)
    {
        id_ = next_id_++;
    }

    // --------------------------------------------------------------------
    RoutedMessageInfo::
        RoutedMessageInfo(RoutedMessageInfo& rmi, bool same_id )
    {
        msg_ = rmi.msg_;
        sender_ = rmi.sender_;
        time_ = rmi.time_;
        round_ = rmi.round_;

        id_ = same_id ? rmi.id_ : next_id_++;
    }

    // --------------------------------------------------------------------
    // unique within each sim run
    long 
        RoutedMessageInfo::
        id()
    {
        return id_;
    }

    // --------------------------------------------------------------------
    shawn::MessageHandle 
        RoutedMessageInfo::
        message()
    {
        return msg_;
    }

    // --------------------------------------------------------------------
    shawn::Node&
        RoutedMessageInfo::
        initial_sender()
    {
        assert(sender_ != NULL);
        return *sender_;
    }

    // --------------------------------------------------------------------
    int 
        RoutedMessageInfo::
        initial_round()
    {
        return round_;
    }

    // --------------------------------------------------------------------
    double 
        RoutedMessageInfo::
        initial_time()
    {
        return time_;
    }


}

#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/routed_message_info.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: routed_message_info.cpp,v $
 *-----------------------------------------------------------------------*/

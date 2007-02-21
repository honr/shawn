/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_ROUTING_ROUTED_MESSAGE_INFO_H
#define __SHAWN_SYS_ROUTING_ROUTED_MESSAGE_INFO_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "sys/message.h"

namespace routing
{
    DECLARE_HANDLES(RoutedMessageInfo);

    /**
      *
      */
    class RoutedMessageInfo : public shawn::RefcntPointable
    {

    public: 
        RoutedMessageInfo(shawn::MessageHandle& msg, shawn::Node& sender, double time, int round);
        RoutedMessageInfo(RoutedMessageInfo& rmi, bool same_id);

        long id();
        shawn::MessageHandle message();
        shawn::Node& initial_sender();
        int initial_round();
        double initial_time();    

    protected:
        long id_;
        shawn::MessageHandle msg_;
        shawn::Node* sender_;
        double time_;
        int round_;

        static long next_id_;
    };

}

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/routed_message_info.h,v $
* Version $Revision: 1.1 $
* Date    $Date: 2005/10/11 15:24:21 $
*-----------------------------------------------------------------------
* $Log: routed_message_info.h,v $
 *-----------------------------------------------------------------------*/

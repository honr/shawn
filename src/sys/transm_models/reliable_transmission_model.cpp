/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/transm_models/reliable_transmission_model.h"
#include "sys/edge_model.h"
#include "sys/world.h"
#include <algorithm>

namespace shawn
{

    // ----------------------------------------------------------------------
    ReliableTransmissionModel::
        ReliableTransmissionModel(bool immediate_delivery_ /* = false */)
        : immediate_delivery_(immediate_delivery_)
    {}

    // ----------------------------------------------------------------------
    ReliableTransmissionModel::
        ~ReliableTransmissionModel()
    {}

    // ----------------------------------------------------------------------
    bool
        ReliableTransmissionModel::
        supports_mobility( void )
        const throw(std::logic_error)
    {
        return world().edge_model().supports_mobility();
    }

    // ----------------------------------------------------------------------
    void
        ReliableTransmissionModel::
        init( void )
        throw()
    {
        TransmissionModel::init();
    }

    // ----------------------------------------------------------------------
    void
        ReliableTransmissionModel::
        reset( void )
        throw()
    {
        TransmissionModel::reset();

        while( !aired_messages_.empty() )
        {
            MessageInfo* mi = aired_messages_.front();
            aired_messages_.pop();
            delete mi;
        }
    }

    // ----------------------------------------------------------------------
    void
        ReliableTransmissionModel::
        send_message( TransmissionModel::MessageInfo& mi )
        throw()
    {
        if( immediate_delivery_ )
            deliver_one_message(mi);
        else
            queued_messages_.push( &mi );
    }

    // ----------------------------------------------------------------------
    void
        ReliableTransmissionModel::
        deliver_messages()
        throw()
    {
        // copy the queued messages to the aired_messages and clear the queue.
        while( !queued_messages_.empty() )
        {
            aired_messages_.push( queued_messages_.front() );
            queued_messages_.pop();
        }

        // Deliver messages. It is possible, that a processor receives a
        // message and resend it immediately. Therefore there are the two
        // queues queued- and aired_messages. The following loop processes
        // the aired_messages_, whereby messages which are sent during the
        // loop, are packed into queued_messages_.
        while( ! aired_messages_.empty() )
        {
            MessageInfo* mi = aired_messages_.front();
            aired_messages_.pop();
            deliver_one_message( *mi );
            delete mi;
        }
    }

    // ----------------------------------------------------------------------
    void
        ReliableTransmissionModel::
        deliver_one_message( TransmissionModel::MessageInfo&mi )
        throw()
    {

        if( mi.msg_->is_unicast() )
        {
            ABORT_INCONSISTENT_CONFIGURATION("Unicast is not supported by the reliable transmission model. Implement it -> NOW.");
        }

        for( EdgeModel::adjacency_iterator it = world_w().begin_adjacent_nodes_w( *mi.src_ ),
             endit = world_w().end_adjacent_nodes_w( *mi.src_ ); it != endit; ++it )
            it->receive( ConstMessageHandle(mi.msg_) );
    }


}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/reliable_transmission_model.cpp,v $
* Version $Revision: 1.8 $
* Date    $Date: 2005/12/01 14:27:46 $
*-----------------------------------------------------------------------
* $Log: reliable_transmission_model.cpp,v $
 *-----------------------------------------------------------------------*/

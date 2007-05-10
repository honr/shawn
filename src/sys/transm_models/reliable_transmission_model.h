/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TRANS_MODELS_RELIABLE_H
#define __SHAWN_SYS_TRANS_MODELS_RELIABLE_H

#include "sys/transmission_model.h"

#include <queue>

namespace shawn
{

    /// The ReliableTransmissionModel (\"reliable\") delivers every message without loss."
    /** If $immediate_delivery is set to true, messages will be delivered immediately, otherwise
    * they will be delivered at the beginning of a new simulation round, just before Processor::work()
    * is called.
    */
    class ReliableTransmissionModel : public TransmissionModel 
    {
    public:
        ///@name Construction, destruction and lifecycle support
        ///@{

        ReliableTransmissionModel(bool = false);
        virtual ~ReliableTransmissionModel();

        ///@}
        ///@name Transmission model implementation
        ///@{

        ///
        virtual void init( void ) throw();
        ///
        virtual void reset( void ) throw();

        ///Mobility is depending on mobility support from the edge model
        /** The edge model is used to determine the 1-hop neighbours
        * which will receive the message 
        */
        virtual bool supports_mobility( void ) const throw(std::logic_error);

        /// Stores each message in a list for delivery at the next simulation round start.
        virtual void send_message( TransmissionModel::MessageInfo& mi ) throw();

        /// Delivers all messages which are in the list
        virtual void deliver_messages() throw();

        ///@}

    private:
        bool immediate_delivery_;

        ///Delivers one message reliably to one node
        virtual void deliver_one_message( TransmissionModel::MessageInfo& mi ) throw();

        ///The messages that have been sent by the nodes and are waiting for delivery
        std::queue<TransmissionModel::MessageInfo*> aired_messages_;
        std::queue<TransmissionModel::MessageInfo*> queued_messages_;

    };

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/reliable_transmission_model.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: reliable_transmission_model.h,v $
 *-----------------------------------------------------------------------*/

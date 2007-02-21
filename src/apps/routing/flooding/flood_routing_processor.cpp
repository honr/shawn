/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "flood_routing_processor.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/flooding/flood_routing_processor.h"
#include "apps/routing/flooding/flood_routing.h"
#include "sys/node.h"

#include <iostream>
using namespace std;

namespace routing 
{
    namespace flood
    {

        //-----------------------------------------------------------------------
        bool 
            FloodRoutingProcessor::
            process_message( const shawn::ConstMessageHandle& msg) 
            throw()
        {
            const FloodRoutingMessage* frm = dynamic_cast<const FloodRoutingMessage*> (msg.get());
            if( frm == NULL )
                return false;

            FloodRoutingInfo* fri = dynamic_cast<FloodRoutingInfo*> ( &frm->info() );
            if( fri == NULL )
                return false;

            fri->flood_routing_instance_->process_message(owner_w(), *frm, *fri);

            return true;
        }
    }
}
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/flooding/flood_routing_processor.cpp,v $
* Version $Revision: 1.1 $
* Date    $Date: 2005/10/11 15:24:21 $
*-----------------------------------------------------------------------
* $Log: flood_routing_processor.cpp,v $
 *-----------------------------------------------------------------------*/

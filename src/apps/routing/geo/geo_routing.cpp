/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/geo/geo_routing.h"
#include "sys/node.h"

#include <iostream>
#include <limits>
using namespace std;

namespace routing 
{
    namespace geo
    {
        //-----------------------------------------------------------------------
        GeoRoutingMessage::
            GeoRoutingMessage(const RoutedMessageInfoHandle& info)
                : info_(info)
        {}

        //-----------------------------------------------------------------------
        RoutedMessageInfo&
            GeoRoutingMessage::
            info() 
            const throw()
        {
            return *info_.get();
        }
        
        //-----------------------------------------------------------------------
        ostream& operator<<(ostream& os, const GeoRoutingMessage& m) 
        {
            os << "GeoRoutingMessage";
            return os;
        }        
        
    }

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/geo/geo_routing.cpp,v $
* Version $Revision: 1.1 $
* Date    $Date: 2007/01/16 15:58:07 $
*-----------------------------------------------------------------------
* $Log: geo_routing.cpp,v $
 *-----------------------------------------------------------------------*/

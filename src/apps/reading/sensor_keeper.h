/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_SENSOR_KEEPER_H
#define __SHAWN_APPS_SENSOR_KEEPER_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "sys/util/handle_keeper.h"
#include "apps/reading/sensor.h"

namespace reading
{

    ///  Keeper that keeps Sensors.
    /**  The SensorKeeper is derived from the HandleKeeper class and
    * manages different types of Sensors.
    */
    class SensorKeeper
        : public shawn::HandleKeeper<Sensor>
    {
    public:
        ///@name construction / destruction
        ///@{
        ///
        SensorKeeper();
        ///
        virtual ~SensorKeeper();
        ///@}
    };


}

#endif
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/sensor_keeper.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------*/


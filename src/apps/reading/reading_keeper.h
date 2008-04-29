/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_READING_KEEPER_H
#define __SHAWN_APPS_READING_KEEPER_H

#include "_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "apps/reading/reading.h"
#include "sys/util/handle_keeper.h"

namespace reading
{

    ///  Keeper that keeps Readings.
    /**  The ReadingKeeper is derived from the HandleKeeper class and
    * manages different types of Readings.
    */
    class ReadingKeeper
        : public shawn::HandleKeeper<Reading>
    {
    public:
        ///@name construction / destruction
        ///@{
        ///
        ReadingKeeper();
        ///
        virtual ~ReadingKeeper();
        ///@}
    };


}

#endif
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/reading_keeper.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: reading_keeper.h,v $
 *-----------------------------------------------------------------------*/


/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_UTIL_KEEPER_MANAGED_H
#define __SHAWN_SYS_UTIL_KEEPER_MANAGED_H

#include "sys/util/refcnt_pointable.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/defutils.h"
#include "sys/logging/logger.h"

namespace shawn
{
   DECLARE_HANDLES(KeeperManaged);

   class KeeperManaged
      : public RefcntPointable
#ifdef ENABLE_LOGGING
        , public Logger
#endif
   {
   public:
      virtual ~KeeperManaged();

      virtual std::string name( void ) const throw() = 0;
      virtual std::string description( void ) const throw() = 0;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/util/keeper_managed.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: keeper_managed.h,v $
 *-----------------------------------------------------------------------*/

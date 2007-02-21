/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_PROTO_HELLOWORLD_MESSAGE_H
#define __SHAWN_PROTO_HELLOWORLD_MESSAGE_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_HELLOWORLD


#include "sys/message.h"

namespace helloworld
{

   class HelloworldMessage
       : public shawn::Message
   {
   public:
      HelloworldMessage();
      virtual ~HelloworldMessage();
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/helloworld/helloworld_message.h,v $
 * Version $Revision: 1.4 $
 * Date    $Date: 2005/08/05 10:00:29 $
 *-----------------------------------------------------------------------
 * $Log: helloworld_message.h,v $
 *-----------------------------------------------------------------------*/

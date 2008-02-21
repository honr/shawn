/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_EXAMPLES_PROCESSOR_HELLOWORLD_MESSAGE_H
#define __SHAWN_EXAMPLES_PROCESSOR_HELLOWORLD_MESSAGE_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_EXAMPLES

#include "sys/message.h"

namespace helloworld
{

   class HelloworldMessage
       : public shawn::Message
   {
   public:
	   HelloworldMessage();
	   HelloworldMessage(int);
	   virtual ~HelloworldMessage();
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/helloworld/helloworld_message.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: helloworld_message.h,v $
 *-----------------------------------------------------------------------*/

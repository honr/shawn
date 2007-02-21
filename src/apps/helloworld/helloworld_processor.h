/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_HELLOWORLD_PROCESSOR_H
#define __SHAWN_APPS_HELLOWORLD_PROCESSOR_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_HELLOWORLD

#include <set>
#include "sys/processor.h"

namespace helloworld
{

   class HelloworldProcessor
       : public shawn::Processor
   {
   public:
      HelloworldProcessor();
      virtual ~HelloworldProcessor();

      virtual void boot( void ) throw();
      virtual bool process_message( const shawn::ConstMessageHandle& ) throw();
      virtual void work( void ) throw();

   private:
      int last_time_of_receive_;
      std::set<const shawn::Node*> neighbours_;
   };


}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/helloworld/helloworld_processor.h,v $
 * Version $Revision: 1.5 $
 * Date    $Date: 2005/08/05 10:00:29 $
 *-----------------------------------------------------------------------
 * $Log: helloworld_processor.h,v $
 *-----------------------------------------------------------------------*/

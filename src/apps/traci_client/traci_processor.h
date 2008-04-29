/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_LEGACYAPPS_TRACI_PROCESSOR_H
#define __SHAWN_LEGACYAPPS_TRACI_PROCESSOR_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_TRACICLIENT

#include <set>
#include "sys/processor.h"

namespace traci
{

   class TraCIProcessor
	   : public shawn::Processor
   {
   public:
      TraCIProcessor();
      virtual ~TraCIProcessor();

      virtual void boot( void ) throw();
      virtual bool process_message( const shawn::ConstMessageHandle& ) throw();
      virtual void work( void ) throw();

	  void disable_node() throw();
	  void enable_node() throw();
	  bool disabled() throw();

   private:
	   bool disabled_;
		
   };


}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/helloworld/helloworld_processor.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: helloworld_processor.h,v $
 *-----------------------------------------------------------------------*/

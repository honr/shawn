/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_STRING_PACKET_H
#define __SHAWN_APPS_STRING_PACKET_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_SPYGLASS

#include "apps/spyglass/spyglass_packet.h"

namespace spyglass
{
   class StringPacket : public SpyglassPacket
   {
   public:
	  StringPacket();
	   virtual ~StringPacket();
	   
	   std::vector<char>* serialize( void ) throw(); 
	   int get_size( void ) throw();
	   void set_string( std::string ) throw();

   protected:
	   std::string str_;

   };
}
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/spyglass/string_packet.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: string_packet.h,v $
 *-----------------------------------------------------------------------*/

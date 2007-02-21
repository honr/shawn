/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "apps/reading/reading.h"

namespace reading
{
   Reading::Reading()
   {}
   // ----------------------------------------------------------------------
   Reading::~Reading()
   {
   	world_ = NULL;
	delete world_;
   }
   // ----------------------------------------------------------------------
   void Reading::set_world( shawn::World& w ) throw()
   {
        world_ = &w;
   }
   // ----------------------------------------------------------------------
   void
   Reading::
   init( void )
      throw()
   {
      assert( world_ != NULL );
   }
   // ----------------------------------------------------------------------
   void Reading::add_changed_handler(ReadingChangedHandler* rch, shawn::Box& b )
   {
	bool pair_exists = false;
	for(ReadingChangedHandlerList::iterator it = rbv.begin(); it != rbv.end(); ++it)
	{	
		if(rch == it->first && b == it->second )
		{ 
			TRACE("READING: <ReadingChangedHandler, Box>-pair already registered");
			pair_exists = true;
		}
	}
	if( !pair_exists )
	{
		//variable which contains the passed parameters in a struct <pair> RB
		RB rb = std::make_pair(rch, b);
		rbv.push_back(rb);
	}
   }
   // ----------------------------------------------------------------------
   void Reading::remove_changed_handler(ReadingChangedHandler* rch, shawn::Box& b ) 
   {
	for(ReadingChangedHandlerList::iterator it = rbv.begin(); it != rbv.end(); ++it)
	{
		if( rch == it->first && b == it->second ) 
		{
		        rbv.erase(it);
			break;
		}
	}
   }
   // ----------------------------------------------------------------------
   bool
   Reading::
   has_world( void )
   const throw()
    {
        return world_ != NULL;
    }
   // ----------------------------------------------------------------------
   const shawn::World& Reading::world( void )const throw()
   {
        return *world_;
   }
   // ----------------------------------------------------------------------
   shawn::World& Reading::world_w( void ) throw()
   {
        return *world_;

   }
}

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/reading.cpp,v $
 * Version $Revision: 1.6 $
 * Date    $Date: 2006/08/31 21:02:26 $
 *-----------------------------------------------------------------------
 * $Log: reading.cpp,v $
 *-----------------------------------------------------------------------*/

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/transmission_model.h"
#include "sys/world.h"
#include "sys/node.h"

namespace shawn
{
  

   TransmissionModel::
   TransmissionModel()
      : world_      ( NULL )
   {}
   // ----------------------------------------------------------------------
   TransmissionModel::
   ~TransmissionModel()
   {}
   // ----------------------------------------------------------------------
   void
   TransmissionModel::
   set_world( World& w ) 
      throw()
   {
      world_ = &w;
   }
   // ----------------------------------------------------------------------
   World&
   TransmissionModel::
   world_w( void ) 
      throw()
   {
      assert( world_ != NULL );
      return *world_;
   }
   // ----------------------------------------------------------------------
   const World&
   TransmissionModel::
   world( void )
      const throw()
   {
      assert( world_ != NULL );
      return *world_;
   }
   // ----------------------------------------------------------------------
   void
   TransmissionModel::
   init( void )
      throw()
   {
      assert( world_ != NULL );
   }
   // ----------------------------------------------------------------------
   void
   TransmissionModel::
   reset( void ) 
      throw()
   {

   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/transmission_model.cpp,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2005/08/05 10:00:37 $
 *-----------------------------------------------------------------------
 * $Log: transmission_model.cpp,v $
 *-----------------------------------------------------------------------*/

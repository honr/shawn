/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_WORLD_FACTORY_H
#define __SHAWN_SYS_WORLD_FACTORY_H

namespace shawn
{
   class World;

   class WorldFactory
   {
   public:
      WorldFactory();
      virtual ~WorldFactory();


      virtual World* create( void ) throw();
      virtual void fill_world( World& ) throw();

   protected:
      virtual World* create_empty_world( void ) throw();
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/world_factory.h,v $
 * Version $Revision: 1.3 $
 * Date    $Date: 2005/08/05 10:00:37 $
 *-----------------------------------------------------------------------
 * $Log: world_factory.h,v $
 *-----------------------------------------------------------------------*/

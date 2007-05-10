/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_WORLDS_HOLE_RECT_WORLD_FACTORY
#define __SHAWN_SYS_WORLDS_HOLE_RECT_WORLD_FACTORY

#include "sys/worlds/rect_world_factory.h"


namespace shawn
{

   class HoleRectWorldFactory
      : public RectWorldFactory
   {
   public:
      HoleRectWorldFactory();
      virtual ~HoleRectWorldFactory();

      virtual void set_hole_size( double, double ) throw();
      virtual double hole_width( void ) const throw();
      virtual double hole_height( void ) const throw();

   protected:
      virtual Vec new_position( void ) const throw();

   private:
      double hole_width_;
      double hole_height_;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/worlds/hole_rect_world_factory.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: hole_rect_world_factory.h,v $
 *-----------------------------------------------------------------------*/

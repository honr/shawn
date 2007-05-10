/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_WORLDS_RECT_WORLD_FACTORY
#define __SHAWN_SYS_WORLDS_RECT_WORLD_FACTORY

#include "sys/worlds/processor_world_factory.h"


namespace shawn
{

   class RectWorldFactory
      : public ProcessorWorldFactory
   {
   public:
      RectWorldFactory();
      virtual ~RectWorldFactory();

      virtual void set_size( double, double ) throw();
      virtual double width( void ) const throw();
      virtual double height( void ) const throw();

   protected:
      virtual Vec new_position( void ) const throw();

   private:
      double width_;
      double height_;
   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/worlds/rect_world_factory.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: rect_world_factory.h,v $
 *-----------------------------------------------------------------------*/

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_SIMPLE_READING_RANDOM_H
#define __SHAWN_APPS_SIMPLE_READING_RANDOM_H
#include "_apps_enable_cmake.h"

#ifdef ENABLE_READING

#include "apps/reading/simple_reading.h"

namespace reading
{
   class DoubleReadingRandom
      : public DoubleReading
   {
   public:
      DoubleReadingRandom();
      virtual ~DoubleReadingRandom();

      virtual double value( const shawn::Vec& v ) const throw();
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

   };
   DECLARE_HANDLES(DoubleReadingRandom);
}

#endif
#endif

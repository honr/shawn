/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "apps/reading/readings/simple_reading_random.h"
#include <time.h>
#include <stdlib.h>

namespace reading
{
   DoubleReadingRandom::DoubleReadingRandom()
   {
      srand(time(NULL));
   }

   DoubleReadingRandom::~DoubleReadingRandom()
   {}

   double DoubleReadingRandom::value( const shawn::Vec &pos )
      const throw()
   {
      double ival = 0.0 + (rand() % 1001);
      ival /= 1000.0;
      return ival;
   }

   std::string
   DoubleReadingRandom::
   name( void )
      const throw()
   {
      return "double_reading_random";
   }
   // ----------------------------------------------------------------------
   std::string
   DoubleReadingRandom::
   description( void )
      const throw()
   {
      return "XXX";
   }


   IntegerReadingRandom::IntegerReadingRandom()
   {
      srand(time(NULL));
   }

   IntegerReadingRandom::~IntegerReadingRandom()
   {}

   std::string
   IntegerReadingRandom::
   name( void )
      const throw()
   {
      return "integer_reading_random";
   }
   // ----------------------------------------------------------------------
   std::string
   IntegerReadingRandom::
   description( void )
      const throw()
   {
      return "XXX";
   }

   int IntegerReadingRandom::value( const shawn::Vec &pos )
      const throw()
   {
      int ival = rand() % 1001;
      return ival;
   }
}

#endif

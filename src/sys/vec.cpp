/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <cmath>
#include <iostream>
#include "sys/vec.h"

namespace shawn
{

   // ----------------------------------------------------------------------
   Vec::~Vec()
   {}
   // ----------------------------------------------------------------------
   double 
   Vec::
   euclidean_norm( void )
      const throw()
   {
      return sqrt( (x_*x_) + (y_*y_) + (z_*z_) ); 
   }
   // ----------------------------------------------------------------------
   std::ostream&
   operator << ( std::ostream& os, const Vec& v )
      throw()
   {
      return os << "(" << v.x() << " " << v.y() << " " << v.z() << ")";
   }

}
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/vec.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: vec.cpp,v $
 *-----------------------------------------------------------------------*/

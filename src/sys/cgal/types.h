/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_CGAL_TYPES_H
#define __SHAWN_SYS_CGAL_TYPES_H
#include "shawn_config.h"
#ifdef HAVE_CGAL

#include <CGAL/Cartesian.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Segment_2.h>

namespace shawn
{

   typedef CGAL::Cartesian<double> CGALKernel;
   typedef CGALKernel::Point_2 CGAL2D;
   typedef CGALKernel::Point_3 CGAL3D;
   typedef CGALKernel::Vector_2 CGALVec2D;
   typedef CGALKernel::Vector_3 CGALVec3D;
   typedef CGALKernel::Line_2 CGALLine2D;
   typedef CGALKernel::Segment_2 CGALSegment2D;
   typedef CGAL::Circle_2<shawn::CGALKernel> CGALCircle2D; 
   typedef CGAL::Polygon_2<CGALKernel> Polygon;

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/cgal/types.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: types.h,v $
 *-----------------------------------------------------------------------*/

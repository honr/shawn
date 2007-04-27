/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TOPOLOGY_POLYGON_TOPOLOGY_H
#define __SHAWN_APPS_TOPOLOGY_POLYGON_TOPOLOGY_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY
#include "shawn_config.h"
#ifdef HAVE_CGAL

#include "sys/cgal/types.h"
#include "apps/topology/topology/topology_2d.h"
#include <CGAL/Polygon_2.h>
#include <vector>

namespace topology
{

   class PolygonTopology
      : public Topology2D
   {
   public:
      typedef CGAL::Polygon_2<shawn::CGALKernel> Polygon;
      typedef std::vector<Polygon*> PolygonVector;
	  typedef std::map<Polygon*, shawn::TagContainer*> PolygonTagMap;

      PolygonTopology();
      virtual ~PolygonTopology();

      virtual void set_name( const std::string& ) throw();
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
      virtual bool value( const shawn::Vec& ) const throw();
      virtual shawn::Box domain( void ) const throw();

      // PolygonTopology will delete this one!
      // passed polygon must be is_simple() !
      virtual void set_outer_polygon( Polygon& ) throw();
      // PolygonTopology will delete the holes!
      // passed polygon must be is_simple() !
      virtual void add_hole_polygon( Polygon& ) throw();

      virtual Polygon& outer();
      virtual PolygonVector& holes();
	  virtual shawn::TagContainer& tags(Polygon& polygon);
      
   private:
      Polygon*      outer_;
      PolygonVector holes_;
      std::string   name_;
	  PolygonTagMap tags_;
   };

}

#endif
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology/polygon_topology.h,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2005/08/05 10:00:35 $
 *-----------------------------------------------------------------------
 * $Log: polygon_topology.h,v $
 *-----------------------------------------------------------------------*/

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY
#include "shawn_config.h"
#ifdef HAVE_CGAL

#include "apps/topology/topology/polygon_topology.h"
using namespace shawn;

namespace topology
{

   PolygonTopology::
   PolygonTopology()
      : outer_( NULL ),
        name_ ("")
   {}
   // ----------------------------------------------------------------------
   PolygonTopology::
   ~PolygonTopology()
   {
      if( outer_ != NULL ) delete outer_;
      for( PolygonVector::iterator it = holes_.begin(), endit=holes_.end();
           it != endit; ++it )
         delete *it;
   }
   // ----------------------------------------------------------------------
   bool
   PolygonTopology::
   value( const Vec& p )
      const throw()
   {
      CGAL2D pc(p);

      assert( outer_ != NULL );

      if( outer_->bounded_side(pc)==CGAL::ON_UNBOUNDED_SIDE )
         return false;

      // we take the boundary of holes to be inside the area, i.e., 
      // the topological closure is considered "inside".
      for( PolygonVector::const_iterator it = holes_.begin(), endit=holes_.end();
           it != endit; ++it )
         if( (**it).bounded_side(pc)==CGAL::ON_BOUNDED_SIDE )
            return false;

      return true;
   }
   // ----------------------------------------------------------------------
   Box
   PolygonTopology::
   domain( void )
      const throw()
   {
      assert( outer_!=NULL );
      CGAL::Bbox_2 bb( outer_->bbox() );
      return Box( Vec( bb.xmin(), bb.ymin(), 0.0 ),
                  Vec( bb.xmax(), bb.ymax(), 0.0 ) );
   }
   // ----------------------------------------------------------------------
   void
   PolygonTopology::
   set_outer_polygon( Polygon& p )
      throw()
   {
      assert( p.is_simple() );
      assert( outer_ == NULL );
      outer_ = &p;
   }
   // ----------------------------------------------------------------------
   void
   PolygonTopology::
   add_hole_polygon( Polygon& p )
      throw()
   {
      assert( p.is_simple() );
      holes_.push_back(&p);
   }
   // ----------------------------------------------------------------------
   std::string
   PolygonTopology::
   name( void )
      const throw()
   {
      return name_;
   }
   // ----------------------------------------------------------------------
   std::string
   PolygonTopology::
   description( void )
      const throw()
   { return std::string("2-dimensional polygonal area"); }
   // ----------------------------------------------------------------------
   void 
   PolygonTopology::
   set_name( const std::string& s )
      throw()
   { name_ = s; }

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology/polygon_topology.cpp,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2005/08/05 10:00:35 $
 *-----------------------------------------------------------------------
 * $Log: polygon_topology.cpp,v $
 * Revision 1.2  2005/08/05 10:00:35  ali
 * 2005 copyright notice
 *
 * Revision 1.1  2005/07/31 17:01:41  ali
 * ported polygon stuff to new topo
 *
 * Revision 1.2  2005/03/02 10:04:28  pfister
 * Topology stuff updated and changes for the new config system
 *
 * Revision 1.1  2005/02/18 19:23:43  ali
 * utilized configbuilder, sys/worlds/topology became apps
 *
 * Revision 1.1  2005/02/06 16:57:08  ali
 * topology generation
 *
 *-----------------------------------------------------------------------*/

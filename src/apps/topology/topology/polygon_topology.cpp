/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY
#include "shawn_config.h"
#ifdef HAVE_CGAL

#include "apps/topology/topology/polygon_topology.h"
using namespace shawn;

namespace topology
{
	// ----------------------------------------------------------------------
	PolygonTopology::
		PolygonTopology()
		: outer_( NULL ),
		name_ ("")
	{}

	// ----------------------------------------------------------------------
	PolygonTopology::
		~PolygonTopology()
	{
		if( outer_ != NULL ) 
			delete outer_;

		//Delete the tags
		for(PolygonTagMap::iterator it = tags_.begin(), endit = tags_.end(); it != endit; ++it)
			delete it->second;

		//Delete the polygons
		for(PolygonVector::iterator it = holes_.begin(), endit = holes_.end(); it != endit; ++it )
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
		for( PolygonVector::const_iterator it = holes_.begin(), endit=holes_.end(); it != endit; ++it )
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
	const shawn::TagContainer*
		PolygonTopology::
		tags(Polygon& polygon) 
		const
	{
		if(tags_.find(&polygon) == tags_.end() )
			return NULL;

		PolygonTagMap& m = *((PolygonTagMap*)(&tags_));//Fieser Hack...
		return m[&(Polygon&)polygon];
	}

	// ----------------------------------------------------------------------
	shawn::TagContainer& 
		PolygonTopology::
		tags_w(Polygon& polygon)
	{
		if(tags_.find(&polygon) == tags_.end() )
			tags_[&polygon] = new TagContainer();

		assert( tags_.find(&polygon) != tags_.end() );
		return *tags_[&polygon];
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
	{ 
		return std::string("2-dimensional polygonal area"); 
	}

	// ----------------------------------------------------------------------
	void 
		PolygonTopology::
		set_name( const std::string& s )
		throw()
	{ 
		name_ = s; 
	}

	// ----------------------------------------------------------------------
	Polygon& 
		PolygonTopology::
		outer_w()
		const
	{
		assert (outer_ != NULL); 
		return *outer_;
	}

	// ----------------------------------------------------------------------
	PolygonTopology::ConstPolygonVector& 
		PolygonTopology::
		holes()
		const
	{
		return holes_;
	}

	// ----------------------------------------------------------------------
	PolygonTopology::PolygonVector& 
		PolygonTopology::
		holes_w()
	{
		return holes_;
	}

}

#endif
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/topology/topology/polygon_topology.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: polygon_topology.cpp,v $
*-----------------------------------------------------------------------*/

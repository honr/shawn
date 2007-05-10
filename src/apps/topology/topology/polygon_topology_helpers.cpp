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

#include "apps/topology/topology/polygon_topology_helpers.h"

#include <CGAL/ch_graham_andrew.h>
#include <iostream>
#include <vector>

using namespace std;
using namespace shawn;

namespace topology
{

    // ----------------------------------------------------------------------
    Polygon& 
        convex_hull(Polygon::Vertex_const_iterator begin, Polygon::Vertex_const_iterator end)
        throw()
    {
        vector <CGAL2D> vertices;
        CGAL::ch_graham_andrew(begin, end, back_inserter(vertices));
        Polygon& pnew = *(new Polygon);

        for(vector <CGAL2D>::iterator it = vertices.begin(); it != vertices.end(); ++it) 
            pnew.push_back( *it );

        return pnew;
    }
    
    // ----------------------------------------------------------------------
    CGALSegment2D shortest_distance_segment(const Polygon& poly, const CGAL2D p) 
        throw()
    {
        double min = numeric_limits<double>::max(), d;
        CGALSegment2D min_segment;

        for(Polygon::Edge_const_iterator it = poly.edges_begin(); it != poly.edges_end() ; ++it)
        {
            d = squared_distance((*it), p);
            if( d < min )
            {
                min = d;
                min_segment = *it;
            }
        }
        return min_segment;
    }


    // ----------------------------------------------------------------------
    double shortest_distance(const Polygon& poly, const CGAL2D p) 
        throw()
    {
        return sqrt( squared_distance( shortest_distance_segment(poly, p), p ));
    }



}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology/polygon_topology_helpers.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: polygon_topology_helpers.cpp,v $
 * Revision 1.2  2005/08/05 10:00:35  ali
 * 2005 copyright notice
 *
 * Revision 1.1  2005/07/31 17:01:42  ali
 * ported polygon stuff to new topo
 *
 * Revision 1.1  2005/03/02 10:04:28  pfister
 * Topology stuff updated and changes for the new config system
 *
 *-----------------------------------------------------------------------*/

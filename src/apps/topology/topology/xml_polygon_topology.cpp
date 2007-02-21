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

#include "apps/topology/topology/xml_polygon_topology.h"
#include "apps/topology/topology/polygon_topology_helpers.h"
#include "sys/util/string_conv.h"

#include <CGAL/ch_graham_andrew.h>

#include <iostream>
#include <iterator>
#include <vector>

using namespace shawn;
using namespace std;

namespace topology
{

   // ----------------------------------------------------------------------
    XMLPolygonTopology::
        XMLPolygonTopology()
        : parsing_state_(UnknownState),
        polygon_type_(HoleType),
        polygon_(NULL),
        outer_(NULL),
        polygon_count_(0),
        create_outer_polygon_(false)
    {}

    // ----------------------------------------------------------------------
    XMLPolygonTopology::
        ~XMLPolygonTopology()
    {}

    // ----------------------------------------------------------------------
    void
        XMLPolygonTopology::
        read( const string& filename, bool create_outer_polygon /* = false */)
        throw( runtime_error )
    {
        this->create_outer_polygon_ = create_outer_polygon;

        if( create_outer_polygon_ ) {
            outer_ = new Polygon;
            cerr << "XMLPolygonTopology: Will create outer convex hull polygon" << endl;
        }

        set_document_uri(filename);
        parse();
        
        if( create_outer_polygon_ )
            set_outer_polygon( topology::convex_hull(outer_->vertices_begin(), outer_->vertices_end()) );

        cerr << "XMLPolygonTopology: Loaded " << polygon_count_ << " polygons" << endl;    
    }

    // ----------------------------------------------------------------------
    void
        XMLPolygonTopology::
        handle_start_element(const char *name, const char **atts) 
        throw(std::runtime_error)
    {
      if( parsing_state_ == UnknownState && !strcmp("topology", name) )
        {
            parsing_state_ = TopologyState;
        }
        else if( parsing_state_ == TopologyState && !strcmp("polygon", name) ) 
        {

            polygon_type_ = polygon_type(atts);
            parsing_state_ = PolygonState;
            polygon_ = new Polygon;
        } 
        else if( parsing_state_ == PolygonState && !strcmp("vertex", name) ) 
        {
            assert( polygon_ != NULL );

            polygon_->push_back( to_point(atts) );

            if( create_outer_polygon_ && polygon_type_ != OuterType )
                outer_->push_back( to_point(atts) );
        }
    }

    // ----------------------------------------------------------------------
    void
        XMLPolygonTopology::
        handle_end_element(const char *name) 
        throw(std::runtime_error)
    {
       if( parsing_state_ == TopologyState && !strcmp("topology", name) )
        {
            parsing_state_ = DoneState;
        }
        else if( parsing_state_ == PolygonState && !strcmp("polygon", name) ) 
        {
            assert( polygon_ != NULL );
            parsing_state_ = TopologyState;
            polygon_count_++;

            //If the input polygon is not simple, a convex hull around the polygons
            //vertices is created instead
            if( ! polygon_->is_simple() ) 
            {
                Polygon& pnew = topology::convex_hull(polygon_->vertices_begin(), polygon_->vertices_end());
                delete polygon_;
                polygon_ = &pnew;
            }

            //Add the polygon to the list of polygons depending on the type specified in the 
            //opening 'polygon' xml tag before
            if( HoleType == polygon_type_ )
                add_hole_polygon( *polygon_  );
            else if( OuterType == polygon_type_ && !create_outer_polygon_ )
                set_outer_polygon( *polygon_  );

            polygon_ = NULL;
        }
    }

    // ----------------------------------------------------------------------
    CGAL2D 
        XMLPolygonTopology::
        to_point(const char** atts) 
        const throw(std::runtime_error)
    {
        double x = conv_string_to_double( attribute("x", atts, "0.0") );
        double y = conv_string_to_double( attribute("y", atts, "0.0") );
        return CGAL2D(x,y);
    }
    
    // ----------------------------------------------------------------------
    XMLPolygonTopology::PolygonType
        XMLPolygonTopology::
        polygon_type(const char** atts) 
        throw(runtime_error)
    {       
        if(!strcmp( attribute("type", atts, "unknown") , "outer"))
            return OuterType;

        if(!strcmp( attribute("type", atts, "unknown") , "hole"))
            return HoleType;

        throw runtime_error("Polygon type unknown");
    }

}
#endif
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/topology/topology/xml_polygon_topology.cpp,v $
* Version $Revision: 1.2 $
* Date    $Date: 2005/08/05 10:00:35 $
*-----------------------------------------------------------------------
* $Log: xml_polygon_topology.cpp,v $
* Revision 1.2  2005/08/05 10:00:35  ali
* 2005 copyright notice
*
* Revision 1.1  2005/07/31 17:01:42  ali
* ported polygon stuff to new topo
*
* Revision 1.2  2005/03/02 10:04:28  pfister
* Topology stuff updated and changes for the new config system
*
* Revision 1.1  2005/02/18 19:23:43  ali
* utilized configbuilder, sys/worlds/topology became apps
*
* Revision 1.3  2005/02/07 15:15:36  pfister
* *** empty log message ***
*
* Revision 1.2  2005/02/07 14:36:57  pfister
* Support to read xml polygon files. Removed the Centerpoint xml parser dependency and the dom parser. Updated readme for windows.
*
* Revision 1.1  2005/02/06 16:57:08  ali
* topology generation
*
*-----------------------------------------------------------------------*/

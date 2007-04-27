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

#include "apps/topology/topology/xml_polygon_topology.h"
#include "apps/topology/topology/polygon_topology_helpers.h"
#include "sys/util/string_conv.h"
#include "sys/simulation/simulation_controller.h"

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
		tag_count_(0),
        create_outer_polygon_(false)
    {}

    // ----------------------------------------------------------------------
    XMLPolygonTopology::
        ~XMLPolygonTopology()
    {}

    // ----------------------------------------------------------------------
    void
        XMLPolygonTopology::
		read(shawn::SimulationController& sc, const string& filename, bool create_outer_polygon /* = false */)
        throw( runtime_error )
    {
        this->create_outer_polygon_ = create_outer_polygon;
		set_tag_factory_keeper( sc.tag_factory_keeper_w() );

        if( create_outer_polygon_ ) {
            outer_ = new Polygon;
            cerr << "XMLPolygonTopology: Will create outer convex hull polygon" << endl;
        }

        set_document_uri(filename);
        parse();

        if( create_outer_polygon_ )
            set_outer_polygon( topology::convex_hull(outer_->vertices_begin(), outer_->vertices_end()) );

        cerr << "XMLPolygonTopology: Loaded " << polygon_count_ << " polygons and " << tag_count_ << " tags" << endl;    
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
		else if( parsing_state_ == PolygonState && !strcmp("tag", name) )
		{
			parsing_state_ = TagState;
			handle_open_tag_tag(atts, tags(*polygon_));
		}
		else if( parsing_state_ == TagState && !strcmp("entry", name) )
		{
			handle_tag_entry(atts);
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
		else if( parsing_state_ == TagState && !strcmp("tag", name) )
		{
			handle_close_tag_tag(NULL, tags(*polygon_));
			parsing_state_ = PolygonState;
			tag_count_++;
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
*-----------------------------------------------------------------------*/

/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#include "shawn_config.h"
#ifdef ENABLE_TOPOLOGY
#ifdef HAVE_CGAL

#include "apps/topology/topology/polygon_topology_postscript_task.h"
#include "apps/topology/topology_keepers.h"
#include "sys/world.h"
#include "sys/simulation/simulation_environment.h"
#include "apps/reading/reading_keeper.h"

using namespace shawn;
using namespace std;
using namespace reading;

namespace topology
{

	// ----------------------------------------------------------------------
	PolygonTopologyPostscriptTask::
		PolygonTopologyPostscriptTask()
		: picture_dimension_ ( 600 ),
		  border_width_ ( 10 )
	{}

	// ----------------------------------------------------------------------
	PolygonTopologyPostscriptTask::
		~PolygonTopologyPostscriptTask()
	{}

	// ----------------------------------------------------------------------
	std::string
		PolygonTopologyPostscriptTask::
		name( void )
		const throw()
	{ 
		return "polygon_topology_postscript"; 
	}

	// ----------------------------------------------------------------------
	std::string
		PolygonTopologyPostscriptTask::
		description( void )
		const throw()
	{ 
		return "";
	}

	// ----------------------------------------------------------------------
	void
		PolygonTopologyPostscriptTask::
		run( shawn::SimulationController& sc )
		throw( std::runtime_error )
	{
#ifndef HAVE_CGAL
		throw std::runtime_error(name() + std::string(" cannot be used: CGAL support disabled at compile-time"));
#else
		string f = sc.environment().required_string_param("file");
		string n = sc.environment().required_string_param("topology");
		PolygonTopology* p = dynamic_cast<PolygonTopology*>(topology_keeper_w(sc).find_w(n).get());

		if( p == NULL )
			throw runtime_error(name() + string(": No PolygonTopology found by name: " + n));

		ofstream outfile(f.c_str());
		if( outfile.bad() )
			throw std::runtime_error(string("Specified file (")+ f +string(") invalid"));

		paint(sc, *p, outfile);

		outfile.flush();
		outfile.close();
#endif
	}


	// ----------------------------------------------------------------------
	void
		PolygonTopologyPostscriptTask::
		paint(shawn::SimulationController& sc, PolygonTopology& p, ostream& out) 
	{
		//Paint parameters
		{
			shawn::Vec ur = sc.world_w().upper_right();
			shawn::Vec ll = sc.world_w().lower_left();
			double wdt= ur.x() - ll.x();
			double hgt= ur.y() - ll.y();
			scale_= (picture_dimension_ - 2.0 * border_width_ ) / wdt;
			double scy = (picture_dimension_ - 2.0 * border_width_) / hgt;

			if( scy < scale_ ) 
				scale_=scy;

			ps_x0_ = border_width_ - (scale_ * ll.x());
			ps_y0_ = border_width_ - (scale_ * ll.y());
		}
		
		write_header(sc, out);

		//TODO: This is a hack
		out << "%%Page: " << "1" << " " << "1" << endl;

		//Topology
		{
			draw(p.outer(), sc, out);
			for(PolygonTopology::PolygonVector::iterator it = p.holes().begin(), end = p.holes().end(); it!=end; ++it)
				draw(**it, sc, out);
		}

		//Connectivity lines
		{
			for(shawn::World::const_node_iterator it = sc.world().begin_nodes(), end = sc.world().end_nodes(); it!=end; ++it)
				for(shawn::Node::const_adjacency_iterator nit = (*it).begin_adjacent_nodes(), nend = (*it).end_adjacent_nodes(); nit!=nend; ++nit)
					if( (*nit).id() < (*it).id() ) 
					{
						Vec pos1  = pos2ps((*it).real_position());
						Vec pos2  = pos2ps((*nit).real_position());
						out << "edgestyle " << pos1.x() << " " << pos1.y() << " moveto " << pos2.x() << " " << pos2.y() << " lineto stroke" << endl;
					}
		}

		//Nodes
		{
			for(shawn::World::const_node_iterator it = sc.world().begin_nodes(), end = sc.world().end_nodes(); it!=end; ++it)
			{
				Vec pos1  = pos2ps((*it).real_position());
				out << pos1.x() << " " << pos1.y() << " " << "normalnode" << endl;         
			}
		}

        out << "showpage" << endl;
	}

	// ----------------------------------------------------------------------
	void
		PolygonTopologyPostscriptTask::
		draw(PolygonTopology::Polygon& p, shawn::SimulationController& sc, ostream& out)
	{
		for(CGAL::Polygon_2<shawn::CGALKernel>::Edge_const_iterator it = p.edges_begin(), end = p.edges_end(); it!=end; ++it)
		{
			CGAL::Polygon_2<shawn::CGALKernel>::Segment_2 edge = *it;
			Vec s = pos2ps(Vec(edge.source())), t = pos2ps(Vec(edge.target()));
			out << "topologystyle " << s.x() << " " << s.y() << " moveto " << t.x() << " " << t.y() << " lineto stroke" << endl;
		}
	}

	// ----------------------------------------------------------------------
	void
		PolygonTopologyPostscriptTask::
		write_header(shawn::SimulationController& sc, std::ostream& out) 
	{
		out
			<< "%!PS-Adobe-2.0" << endl
			<< "%%BoundingBox: 0 0 " << picture_dimension_ << " " << picture_dimension_ << endl
			<< "%%Pages: (atend)" << endl
			<< "%%PageOrder: Ascend" << endl
			<< "%%EndComments" << endl

			<< "/normalnode {" << endl
			<< "  gsave" << endl
			<< "    translate" << endl
			<< "    [ ] 0 setdash " << endl
			<< "    0 setgray" << endl
			<< "    newpath 0 0 1.5 0 360 arc closepath fill" << endl
			<< "  grestore" << endl
			<< "} bind def" << endl

			<< "/edgestyle {" << endl
			<< " [ ] 0 setdash " << endl
			<< "  .3 setgray" << endl
			<< "  1 setlinewidth" << endl
			<< "} bind def" << endl

			<< "/topologystyle {" << endl
			<< " [ ] 0 setdash " << endl
			<< "  .3 setgray" << endl
			<< "  5 setlinewidth" << endl
			<< "} bind def" << endl;
	}

	// ----------------------------------------------------------------------
	Vec
		PolygonTopologyPostscriptTask::
		pos2ps( const Vec& v ) 
		const throw()
	{
		return Vec( (v.x() * scale_) + ps_x0_, (v.y() * scale_) + ps_y0_ );
	}

	// ----------------------------------------------------------------------
	double
		PolygonTopologyPostscriptTask::
		scale(double d) 
		const throw()
	{
		return d * scale_;
	}

}
#endif
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/topology/topology/xml_polygon_topology_task.cpp,v $
* Version $Revision: 1.3 $
* Date    $Date: 2006/04/19 11:03:16 $
*-----------------------------------------------------------------------
* $Log: xml_polygon_topology_task.cpp,v $
*-----------------------------------------------------------------------*/
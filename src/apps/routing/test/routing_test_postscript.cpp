/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/test/routing_test_postscript.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/vec.h"
#include "sys/world.h"
#include "sys/node.h"
#include <fstream>

using namespace std;
using namespace shawn;

namespace routing
{
	static inline const shawn::Vec node_pos(const shawn::Node& n) 
	{
		return n.has_est_position() ? n.est_position() : n.real_position();
	}

	// --------------------------------------------------
	RoutingTestPostscript::
		RoutingTestPostscript(std::ostream& outfile)
		: outfile_(outfile),
		picture_dimension_(600),
		border_width_(10),
		page_count_(0)
	{
		if( outfile_.bad() )
			throw std::runtime_error("Specified file invalid");
	}

	// --------------------------------------------------
	RoutingTestPostscript::
		~RoutingTestPostscript()
	{
		outfile_	<< "%%Trailer" << endl
					<< "%%Pages: " << page_count_ << endl
					<< "%%EOF" << endl;

		outfile_.flush();
		delete &outfile_;
	}

	// --------------------------------------------------
	void 
		RoutingTestPostscript::
		draw_source_node(const shawn::Node& n)
	{
		Vec p  = pos2ps(node_pos(n));

		outfile_ << p.x() << " " << p.y() << " " << "sourcenode" << endl;     
	}

	// --------------------------------------------------
	void 
		RoutingTestPostscript::
		draw_dest_node(const shawn::Node& n)
	{
		Vec p  = pos2ps(node_pos(n));

		outfile_ << p.x() << " " << p.y() << " " << "destnode" << endl;     
	}

	// --------------------------------------------------
	void 
		RoutingTestPostscript::
		draw_msg_path(const shawn::Node& receiver, shawn::ConstMessageHandle m)
	{
		if( m->has_source() )
		{
			Vec pos1 = pos2ps( node_pos(m->source()) );
			Vec pos2 = pos2ps( node_pos(receiver) );

			outfile_ << "msgpathstyle " << pos1.x() << " " << pos1.y() 
				<< " moveto " << pos2.x() << " " << pos2.y() 
				<< " lineto stroke" << endl;
		}
	}

	// --------------------------------------------------
	void 
		RoutingTestPostscript::
		draw_nodes(shawn::World::const_node_iterator begin, shawn::World::const_node_iterator end, bool draw_topology)  
		throw()
	{
		//Connectivity lines
		if( draw_topology )
		{
			for(shawn::World::const_node_iterator it = begin; it!=end; ++it)
			{
				const shawn::Node& n1 = *it;
				Vec pos1  = pos2ps(node_pos(n1));
				for(shawn::Node::const_adjacency_iterator nit = n1.begin_adjacent_nodes(), nend = n1.end_adjacent_nodes(); nit!=nend; ++nit)
					if( (*nit).id() < n1.id() ) 
					{
						Vec pos2  = pos2ps(node_pos((*nit)));

						outfile_ << "edgestyle " << pos1.x() << " " << pos1.y() 
							<< " moveto " << pos2.x() << " " << pos2.y() 
							<< " lineto stroke" << endl;
					}
			}
		}

		//Nodes
		for(shawn::World::const_node_iterator it = begin; it!=end; ++it)
		{
			const shawn::Node& n1 = *it;
			Vec pos1  = pos2ps(node_pos(n1));
			outfile_ << pos1.x() << " " << pos1.y() << " " << "normalnode" << endl;         
		}
	}


	// ----------------------------------------------------------------------
	void
		RoutingTestPostscript::
		start_page()
		throw( std::runtime_error )
	{
		++page_count_;
		outfile_ << "%%Page: " << page_count_ << " " << page_count_ << endl;
	}   
	// ----------------------------------------------------------------------
	void
		RoutingTestPostscript::
		end_page()
		throw( std::runtime_error )
	{
		outfile_ << "showpage" << endl;
	}

	// ----------------------------------------------------------------------
	void
		RoutingTestPostscript::
		custom(std::string cmd) 
		throw()
	{
		outfile_ << cmd << endl;
	}

	// ----------------------------------------------------------------------
	void
		RoutingTestPostscript::
		print_header( SimulationController& sc)
		throw( std::runtime_error )
	{
		assert(outfile_ != NULL);
		outfile_ 
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

			<< "/sourcenode {" << endl
			<< "  gsave" << endl
			<< "    translate" << endl
			<< "    [ ] 0 setdash " << endl
			<< "    1 0 0 setrgbcolor" << endl
			<< "    newpath 0 0 3 0 360 arc closepath fill" << endl
			<< "  grestore" << endl
			<< "} bind def" << endl

			<< "/destnode {" << endl
			<< "  gsave" << endl
			<< "    translate" << endl
			<< "    [ ] 0 setdash " << endl
			<< "    0 0 1 setrgbcolor" << endl
			<< "    newpath 0 0 3 0 360 arc closepath fill" << endl
			<< "  grestore" << endl
			<< "} bind def" << endl

			<< "/edgestyle {" << endl
			<< " [ ] 0 setdash " << endl
			<< "  .9 setgray" << endl
			<< "  0.3 setlinewidth" << endl
			<< "} bind def" << endl

			<< "/msgpathstyle {" << endl
			<< " [ ] 0 setdash " << endl
			<< "  1 0.6 0 setrgbcolor" << endl
			<< "  0.1 setlinewidth" << endl
			<< "} bind def" << endl
			;

		shawn::Vec ur = shawn::Vec(-numeric_limits<double>::max(),-numeric_limits<double>::max(),-numeric_limits<double>::max());
		shawn::Vec ll = shawn::Vec(numeric_limits<double>::max(),numeric_limits<double>::max(),numeric_limits<double>::max());

		for(shawn::World::const_node_iterator it = sc.world().begin_nodes(), end = sc.world().end_nodes(); it!=end; ++it)
		{
			const Vec& p = (*it).real_position();
			ll = shawn::Vec( min(ll.x(),p.x()), min(ll.y(),p.y()), min(ll.z(),p.z()) );
			ur = shawn::Vec( max(ur.x(),p.x()), max(ur.y(),p.y()), max(ur.z(),p.z()) );
		}

		double wdt = ur.x() - ll.x();
		double hgt = ur.y() - ll.y();
		scale_ = (picture_dimension_ - 2.0 * border_width_) / wdt;

		double scy = (picture_dimension_ - 2.0 * border_width_) / hgt;
		if( scy < scale_ ) 
			scale_ = scy;

		ps_x0_ = border_width_ - (scale_*ll.x());
		ps_y0_ = border_width_ - (scale_*ll.y());
	}

	// ----------------------------------------------------------------------
	Vec
		RoutingTestPostscript::
		pos2ps( const Vec& v ) 
		const throw()
	{
		return Vec( (v.x() * scale_) + ps_x0_, (v.y() * scale_) + ps_y0_ );
	}

	// ----------------------------------------------------------------------
	double
		RoutingTestPostscript::
		scale(double d) 
		const throw()
	{
		return d * scale_;
	}
}
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/geo/test/geo_postscript.cpp,v $
* Version $Revision: 1.1 $
* Date    $Date: 2007/01/16 15:58:07 $
*-----------------------------------------------------------------------
* $Log: geo_postscript.cpp,v $
*-----------------------------------------------------------------------*/

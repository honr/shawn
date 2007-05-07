/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_TEST_ROUTING_TEST_POSTSCRIPT_H
#define __SHAWN_APPS_ROUTING_TEST_ROUTING_TEST_POSTSCRIPT_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "sys/simulation/simulation_task.h"
#include "sys/world.h"
#include <iostream>
#include <string>
#include <list>

namespace shawn 
{
	class Node;
	class Vec;
}

namespace routing
{

	class RoutingTestPostscript
	{
	public:
		RoutingTestPostscript(std::ostream& outfile);
		virtual ~RoutingTestPostscript();

		virtual void print_header( shawn::SimulationController&) throw( std::runtime_error );
		virtual void start_page() throw( std::runtime_error );
		virtual void end_page() throw( std::runtime_error );
		virtual void draw_nodes(shawn::World::const_node_iterator begin, shawn::World::const_node_iterator end, bool draw_topology) throw();
		virtual void draw_source_node(const shawn::Node& n);
		virtual void draw_dest_node(const shawn::Node& n);
		virtual void draw_msg_path(const shawn::Node& receiver, shawn::ConstMessageHandle);
		virtual void custom(std::string) throw();
		virtual shawn::Vec pos2ps( const shawn::Vec& ) const throw();
		virtual double scale(double) const throw();

	private:
		std::ostream& outfile_;
		double picture_dimension_;
		double border_width_;
		int page_count_;
		double scale_;
		double ps_x0_;
		double ps_y0_;
	};

}

#endif
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/geo/test/geo_postscript.h,v $
* Version $Revision: 1.1 $
* Date    $Date: 2007/01/16 15:58:07 $
*-----------------------------------------------------------------------
* $Log: geo_postscript.h,v $
*-----------------------------------------------------------------------*/

/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/topology/xml_polygon_topology_task.h"
#include "apps/topology/topology_keepers.h"
#include "sys/simulation/simulation_environment.h"
#include "apps/reading/reading_keeper.h"

#include "shawn_config.h"
#ifdef HAVE_CGAL
#include "apps/topology/topology/xml_polygon_topology.h"
#endif

using namespace shawn;
using namespace reading;

namespace topology
{

	// ----------------------------------------------------------------------
	XMLPolygonTopologyTask::
		XMLPolygonTopologyTask()
	{}

	// ----------------------------------------------------------------------
	XMLPolygonTopologyTask::
		~XMLPolygonTopologyTask()
	{}

	// ----------------------------------------------------------------------
	std::string
		XMLPolygonTopologyTask::
		name( void )
		const throw()
	{ 
		return "xml_polygon_topology"; 
	}

	// ----------------------------------------------------------------------
	std::string
		XMLPolygonTopologyTask::
		description( void )
		const throw()
	{ 
		return "Creates a 2D polygonal topology named $name from file $file. "
			"The optional parameter create_outer_hull (or for compatibility xml_create_outer_hull)"
			"creates the outer polygon automatically.";
	}

	// ----------------------------------------------------------------------
	void
		XMLPolygonTopologyTask::
		run( shawn::SimulationController& sc )
		throw( std::runtime_error )
	{
		#ifndef HAVE_CGAL
		throw std::runtime_error(name() + std::string(" cannot be used: CGAL support disabled at compile-time"));
		#else

			std::string f = sc.environment().required_string_param("file");
			std::string n = sc.environment().required_string_param("name");
			bool create_outer = 
				sc.environment().optional_bool_param("xml_create_outer_hull", false) ||
				sc.environment().optional_bool_param("create_outer_hull", false);

			XMLPolygonTopology* p = new XMLPolygonTopology;
			p->set_name(n);
			p->read(sc, f, create_outer);
			topology_keeper_w(sc).add(p);

		#endif
	}

}
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/topology/topology/xml_polygon_topology_task.cpp,v $
* Version $Revision: 1.3 $
* Date    $Date: 2006/04/19 11:03:16 $
*-----------------------------------------------------------------------
* $Log: xml_polygon_topology_task.cpp,v $
*-----------------------------------------------------------------------*/

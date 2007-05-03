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

#include "apps/topology/comm_models/polygon_comm_model_factory.h"
#include "apps/topology/comm_models/polygon_comm_model.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"

#include <limits>

using namespace std;

namespace topology
{

	// ----------------------------------------------------------------------
	PolygonTopologyCommunicationModelFactory::
		~PolygonTopologyCommunicationModelFactory()
	{}

	// ----------------------------------------------------------------------
	std::string 
		PolygonTopologyCommunicationModelFactory::
		name(void) 
		const throw()
	{
		return "polygon_topology";
	}

	// ----------------------------------------------------------------------
	std::string 	
		PolygonTopologyCommunicationModelFactory::
		description(void) 
		const throw()
	{
		return "";
	}

	// ----------------------------------------------------------------------
	CommunicationModel* 
		PolygonTopologyCommunicationModelFactory::
		create( const SimulationController& sc) 
		const throw()
	{
		double range = sc.environment().optional_double_param("range", std::numeric_limits<int>::max() );
		PolygonTopologyCommunicationModel* pcm = new PolygonTopologyCommunicationModel();

		//TODO: DENNIS: Parametrize the model

		return pcm;
	}


}

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/comm_models/disk_graph_communication_model_factory.cpp,v $
* Version $Revision: 1.1 $
* Date    $Date: 2005/08/12 12:39:59 $
*-----------------------------------------------------------------------
* $Log: disk_graph_communication_model_factory.cpp,v $
 *-----------------------------------------------------------------------*/

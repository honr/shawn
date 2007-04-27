/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/

#include "apps/topology/comm_models/polygon_comm_model.h"
#include "sys/node.h"
#include "sys/world.h"

#include <cassert>
#include <iostream>

using namespace std;
using namespace shawn;

namespace topology
{

	PolygonTopologyCommunicationModel::
		PolygonTopologyCommunicationModel()
	{}

	// ----------------------------------------------------------------------
	PolygonTopologyCommunicationModel::
		~PolygonTopologyCommunicationModel()
	{}

	// ----------------------------------------------------------------------
	void
		PolygonTopologyCommunicationModel::
		init( void )
		throw()
	{
		CommunicationModel::init();
	}
	
	// ----------------------------------------------------------------------
	bool
		PolygonTopologyCommunicationModel::
		can_communicate_bidi( const Node& u,
		const Node& v )
		const throw()
	{
		return false;
	}

	// ----------------------------------------------------------------------
	bool
		PolygonTopologyCommunicationModel::
		can_communicate_uni( const Node& u,
		const Node& v )
		const throw()
	{
		return false;
	}

	// ----------------------------------------------------------------------
	bool
		PolygonTopologyCommunicationModel::
		exists_communication_upper_bound( void )
		const throw()
	{
		return false;
	}

	// ----------------------------------------------------------------------
	double
		PolygonTopologyCommunicationModel::
		communication_upper_bound( void )
		const throw()
	{
		return 0.0;
	}

	// ----------------------------------------------------------------------
	bool
		PolygonTopologyCommunicationModel::
		is_status_available_on_construction( void )
		const throw()
	{
		return false;
	}

	// ----------------------------------------------------------------------
	void 
		PolygonTopologyCommunicationModel::
		set_size_hint(double size_hint)
		throw()
	{
	}

}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/comm_models/disk_graph_model.cpp,v $
* Version $Revision: 1.3 $
* Date    $Date: 2005/08/05 10:00:37 $
*-----------------------------------------------------------------------
* $Log: disk_graph_model.cpp,v $
*-----------------------------------------------------------------------*/

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_TOPOLOGY_COMM_MODELS_TOPOLOGY_COMM_MODEL_FACTORY_H
#define __SHAWN_APPS_TOPOLOGY_COMM_MODELS_TOPOLOGY_COMM_MODEL_FACTORY_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY
#include "shawn_config.h"
#ifdef HAVE_CGAL

#include "apps/topology/topology/topology_2d.h"
#include <CGAL/Polygon_2.h>
#include "sys/cgal/types.h"
#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/defutils.h"
#include "sys/comm_models/communication_model_factory.h"
#include <vector>
#include <string>
#include <vector>

using namespace shawn;

namespace shawn
{
   class CommunicationModel;
   class SimulationController;
}

namespace topology
{

	//----------------------------------------------------------------------------
	/**
	  *
	  */
	class PolygonTopologyCommunicationModelFactory
		: public CommunicationModelFactory
	{
	public:
		virtual ~PolygonTopologyCommunicationModelFactory();
		virtual CommunicationModel* create( const SimulationController& ) const throw();
		virtual std::string name(void)  const throw();
		virtual std::string description(void) const throw();
	};

}

#endif
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source$
 * Version $Revision: 1.2 $
 * Date    $Date: 2006/07/12 10:28:22 $
 *-----------------------------------------------------------------------
 * $Log: disk_graph_communication_model_factory.h,v $
 *-----------------------------------------------------------------------*/

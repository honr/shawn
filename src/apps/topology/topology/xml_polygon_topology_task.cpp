/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
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
   { return "xml_polygon_topology"; }
   // ----------------------------------------------------------------------
   std::string
   XMLPolygonTopologyTask::
   description( void )
      const throw()
   { return "creates a 2D polygonal topology named $name from file $file"; }
   // ----------------------------------------------------------------------
   void
   XMLPolygonTopologyTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
#ifndef HAVE_CGAL
      throw std::runtime_error(name() + 
                               std::string(" cannot be used: CGAL support disabled at compile-time"));
#else
      std::string f = sc.environment().required_string_param("file");
      std::string n = sc.environment().required_string_param("name");


      bool create_outer = sc.environment().optional_bool_param("xml_create_outer_hull", false);
      XMLPolygonTopology* p = new XMLPolygonTopology;
      p->set_name(n);
      p->read(f, create_outer);
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
 * Revision 1.3  2006/04/19 11:03:16  cbuschma
 * *** empty log message ***
 *
 * Revision 1.2  2005/08/05 10:00:36  ali
 * 2005 copyright notice
 *
 * Revision 1.1  2005/07/31 17:01:42  ali
 * ported polygon stuff to new topo
 *
 *-----------------------------------------------------------------------*/

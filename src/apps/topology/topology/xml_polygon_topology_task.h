/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/

#ifndef __SHAWN_APPS_TOPOLOGY_XML_POLYGON_TOPOLOGY_TASK_H
#define __SHAWN_APPS_TOPOLOGY_XML_POLYGON_TOPOLOGY_TASK_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"

namespace topology
{

   class XMLPolygonTopologyTask
      : public shawn::SimulationTask
   {
   public:
      XMLPolygonTopologyTask();
      virtual ~XMLPolygonTopologyTask();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

      virtual void run( shawn::SimulationController& sc )
         throw( std::runtime_error );
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology/xml_polygon_topology_task.h,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2005/08/05 10:00:36 $
 *-----------------------------------------------------------------------
 * $Log: xml_polygon_topology_task.h,v $
 * Revision 1.2  2005/08/05 10:00:36  ali
 * 2005 copyright notice
 *
 * Revision 1.1  2005/07/31 17:01:42  ali
 * ported polygon stuff to new topo
 *
 *-----------------------------------------------------------------------*/

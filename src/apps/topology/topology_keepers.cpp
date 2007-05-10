/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/topology_keepers.h"
#include "apps/topology/point_gen/point_gen_keeper.h"
#include "apps/topology/node_gen/node_gen_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "apps/reading/reading_keeper.h"

using namespace reading;

namespace topology
{

	const reading::ReadingKeeper&
   elevation_keeper( const shawn::SimulationController& sc )
      throw()
   { return sc.reading_keeper(); }
   // ----------------------------------------------------------------------
   reading::ReadingKeeper&
   elevation_keeper_w( shawn::SimulationController& sc )
      throw()
   { return sc.reading_keeper_w(); }
   // ----------------------------------------------------------------------
   const reading::ReadingKeeper& 
   topology_keeper( const shawn::SimulationController& sc )
      throw()
   { return sc.reading_keeper(); }
   // ----------------------------------------------------------------------
   reading::ReadingKeeper& 
   topology_keeper_w( shawn::SimulationController& sc ) 
      throw()
   { return sc.reading_keeper_w(); }
   // ----------------------------------------------------------------------
   const PointGeneratorKeeper& 
   point_gen_keeper( shawn::SimulationController& sc )
      throw()
   {
      const PointGeneratorKeeper* pgk =
         sc.keeper_by_name<PointGeneratorKeeper>(PointGeneratorKeeper::KEEPER_NAME);
      assert( pgk != NULL );
      return *pgk;
   }
   // ----------------------------------------------------------------------
   PointGeneratorKeeper& 
   point_gen_keeper_w( shawn::SimulationController& sc )
      throw()
   {
      PointGeneratorKeeper* pgk =
         sc.keeper_by_name_w<PointGeneratorKeeper>(PointGeneratorKeeper::KEEPER_NAME);
      assert( pgk != NULL );
      return *pgk;
   }
   // ----------------------------------------------------------------------
   const NodeGeneratorKeeper& 
   node_gen_keeper( shawn::SimulationController& sc )
      throw()
   {
      const NodeGeneratorKeeper* pgk =
         sc.keeper_by_name<NodeGeneratorKeeper>(NodeGeneratorKeeper::KEEPER_NAME);
      assert( pgk != NULL );
      return *pgk;
   }
   // ----------------------------------------------------------------------
   NodeGeneratorKeeper& 
   node_gen_keeper_w( shawn::SimulationController& sc )
      throw()
   {
      NodeGeneratorKeeper* pgk =
         sc.keeper_by_name_w<NodeGeneratorKeeper>(NodeGeneratorKeeper::KEEPER_NAME);
      assert( pgk != NULL );
      return *pgk;
   }




}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology_keepers.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: topology_keepers.cpp,v $
 *-----------------------------------------------------------------------*/

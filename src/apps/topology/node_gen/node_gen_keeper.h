/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TOPOLOGY_NODE_GEN_KEEPER_H
#define __SHAWN_APPS_TOPOLOGY_NODE_GEN_KEEPER_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "sys/util/handle_keeper.h"
#include "apps/topology/node_gen/node_gen.h"
#include <string>


namespace topology
{
   

   class NodeGeneratorKeeper
      : public shawn::HandleKeeper<NodeGenerator>
   {
   public:
      static const std::string KEEPER_NAME;

      NodeGeneratorKeeper();
      virtual ~NodeGeneratorKeeper();
   };


}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/node_gen/node_gen_keeper.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: node_gen_keeper.h,v $
 *-----------------------------------------------------------------------*/

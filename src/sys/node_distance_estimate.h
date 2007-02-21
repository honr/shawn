/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_NODE_DISTANCE_ESTIMATE_H
#define __SHAWN_SYS_NODE_DISTANCE_ESTIMATE_H

#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/defutils.h"


namespace shawn
{
   class Node;

   DECLARE_HANDLES(NodeDistanceEstimate);

   class NodeDistanceEstimate
      : public KeeperManaged
   {
   public:
      virtual ~NodeDistanceEstimate();
      
      virtual bool estimate_distance( const Node& source,
                                      const Node& target,
                                      double& result )
         const throw() = 0;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/node_distance_estimate.h,v $
 * Version $Revision: 1.3 $
 * Date    $Date: 2005/08/05 10:00:36 $
 *-----------------------------------------------------------------------
 * $Log: node_distance_estimate.h,v $
 *-----------------------------------------------------------------------*/

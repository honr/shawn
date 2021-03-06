/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_UTIL_NODE_FINDER_H
#define __SHAWN_SYS_UTIL_NODE_FINDER_H

#include "sys/node.h"
#include "sys/world.h"
#include "sys/vec.h"

#include <set>


namespace shawn
{
		typedef std::set<Node*> NodeSet;
   
   class NodeFinder
   {
   public:
		NodeFinder(World* w);
		Node* findByPosition( Vec v);
		NodeSet findInCircle( Vec center, double radius );
		Node* findById( int id );

   private:

		World* w_;

		double dist( Vec, Vec );

    };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/node_finder.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: node_finder.h,v $
 *-----------------------------------------------------------------------*/

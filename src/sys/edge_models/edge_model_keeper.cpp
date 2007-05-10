/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <iostream>
#include "sys/edge_models/edge_model_keeper.h"

using namespace std;

namespace shawn
{
   EdgeModelKeeper::
   EdgeModelKeeper()
      : HandleKeeper<EdgeModelFactory>( "EdgeModelKeeper", "edge_models" )
   {}
   // ----------------------------------------------------------------------
   EdgeModelKeeper::
   ~EdgeModelKeeper()
   {}


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/edge_models/edge_model_keeper.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: edge_model_keeper.cpp,v $
 *-----------------------------------------------------------------------*/

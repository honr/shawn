/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <iostream>
#include "sys/comm_models/communication_model_factory.h"
#include "sys/comm_models/communication_model_keeper.h"

using namespace std;

namespace shawn
{
   CommunicationModelKeeper::
   CommunicationModelKeeper()
      : HandleKeeper<CommunicationModelFactory>( "CommunicationModelKeeper", "communication_models" )
   {}
   // ----------------------------------------------------------------------
   CommunicationModelKeeper::
   ~CommunicationModelKeeper()
   {}

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/comm_models/communication_model_keeper.cpp,v $
 * Version $Revision: 1.1 $
 * Date    $Date: 2005/08/12 12:39:59 $
 *-----------------------------------------------------------------------
 * $Log: communication_model_keeper.cpp,v $
 *-----------------------------------------------------------------------*/

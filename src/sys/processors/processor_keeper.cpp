/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <iostream>
#include "sys/processors/processor_keeper.h"

using namespace std;

namespace shawn
{
   ProcessorKeeper::
   ProcessorKeeper()
      : HandleKeeper<ProcessorFactory>( "ProcessorKeeper", "processors" )
   {}
   // ----------------------------------------------------------------------
   ProcessorKeeper::
   ~ProcessorKeeper()
   {}



}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/processors/processor_keeper.cpp,v $
 * Version $Revision: 1.4 $
 * Date    $Date: 2005/08/05 10:00:41 $
 *-----------------------------------------------------------------------
 * $Log: processor_keeper.cpp,v $
 *-----------------------------------------------------------------------*/

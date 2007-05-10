/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/misc/random/random_variable_keeper.h"
#include "sys/misc/random/random_variable.h"

namespace shawn
{


   RandomVariableKeeper::
   RandomVariableKeeper()
      : HandleKeeper<RandomVariable>( "RandomVariableKeeper", "random_variables" )
   {}
   // ----------------------------------------------------------------------
   RandomVariableKeeper::
   ~RandomVariableKeeper()
   {}


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/random/random_variable_keeper.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: random_variable_keeper.cpp,v $
 *-----------------------------------------------------------------------*/

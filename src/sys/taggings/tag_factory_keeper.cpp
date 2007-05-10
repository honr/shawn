/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/taggings/tag_factory_keeper.h"


namespace shawn
{

   TagFactoryKeeper::
   TagFactoryKeeper()
      : HandleKeeper<TagFactory>( "TagFactoryKeeper", "tags" )
   {}
   // ----------------------------------------------------------------------
   TagFactoryKeeper::
   ~TagFactoryKeeper()
   {}

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/taggings/tag_factory_keeper.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: tag_factory_keeper.cpp,v $
 *-----------------------------------------------------------------------*/

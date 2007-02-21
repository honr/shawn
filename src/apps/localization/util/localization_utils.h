/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_UTIL_UTILS_H
#define __SHAWN_APPS_LOCALIZATION_UTIL_UTILS_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include <string>


namespace localization
{

   ///
   int get_index( const std::string&, const std::string[], int, int )
      throw();

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/util/localization_utils.h,v $
 * Version $Revision: 1.4 $
 * Date    $Date: 2005/08/05 10:00:33 $
 *-----------------------------------------------------------------------
 * $Log: localization_utils.h,v $
 *-----------------------------------------------------------------------*/

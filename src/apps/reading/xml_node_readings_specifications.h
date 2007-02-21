/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef  __SHAWN_APPS_XML_NODE_READINGS_SPECIFICATIONS_H
#define __SHAWN_APPS_XML_NODE_READINGS_SPECIFICATIONS_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "apps/reading/xml_readings_specifications.h"
#include "sys/vec.h"

#include <map>
#include <list>

namespace xmlreading
{
	/*struct StringComp
	{
		bool operator () (const std::string& s1, const std::string& s2) const
   		{
       			return (s1 == s2);
   		}
 	};*/
	// ----------------------------------------------------------------------
	typedef std::map<int, Values> NodeReadings;
	// ----------------------------------------------------------------------
}

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/reading/xml_node_readings_specifications.h,v $
* Version $Revision: 1.3 $
* Date    $Date: 2006/04/13 17:30:35 $
*-----------------------------------------------------------------------*/


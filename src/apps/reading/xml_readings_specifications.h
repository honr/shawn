/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef  __SHAWN_APPS_XML_READINGS_SPECIFICATIONS_H
#define __SHAWN_APPS_XML_READINGS_SPECIFICATIONS_H

#include "_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "sys/vec.h"

#include <map>
#include <list>

namespace xmlreading
{
	///@name reading specifications
	///@{
	/** Used to sort a map with vectors as key values */
	struct VecComparison
 	{
   		bool operator () (const shawn::Vec& v1, const shawn::Vec& v2) const
   		{
       			if(v1.x() < v2.x())
			{
				return true;
			}else if (v1.x() == v2.x())
			{
				if(v1.y() < v2.y())
				{
					return true;
				}else if(v1.y() == v2.y())
				{
					if(v1.z() < v2.z())
					{
						return true;
					}else return false;
				}else return false;
			}else return false;
		}
 	};
	///
	/** Holds the information regarding what type (simple, complex) and what data (int, double, string, bool) the reading should have */
	struct ReadingData
	{
		std::string type;
		std::string data;
	};
	///
	/** Holds information regarding what kind of setting you want to use in processing the reading information and an additional value variable you can use to do some more stuff*/
	struct SettingData
	{
		std::string name;
		std::string value;
	};
	///
	typedef std::pair<std::string, double> Values;
	///
	typedef std::map<shawn::Vec, Values, VecComparison> Readings;
	///
	typedef std::list<struct SettingData*> Settings;
	///@}
}

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/reading/xml_readings_specifications.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------*/


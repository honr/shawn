/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_XML_NODE_SENSOR_BASE_H
#define __SHAWN_APPS_XML_NODE_SENSOR_BASE_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef	ENABLE_READING

//#define MAX_DOUBLE 1E+300 	// Definition for infinite double value
#ifndef TRACE_READINGS
#define TRACE_READINGS(x) x			//show all reading maps
#endif

#include "apps/reading/xml_node_readings_specifications.h"

#include <limits>

namespace xmlreading
{
	class XMLNodeSensorBase 
	{
	public:
		XMLNodeSensorBase();
		//-----------------------------------------------------------------------
		virtual ~XMLNodeSensorBase();
		//-----------------------------------------------------------------------
		virtual void refresh_value_durations() throw() = 0;
		//-----------------------------------------------------------------------
		virtual void receive_settings(xmlreading::Settings&, std::string&) throw() = 0;
		//-----------------------------------------------------------------------
		virtual void receive_new_values(xmlreading::NodeReadings&) throw() = 0;
		//-----------------------------------------------------------------------
		virtual void set_next_event() throw() = 0;
		//-----------------------------------------------------------------------
	public:
		double current_time_;
	};
}

#endif 
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/reading/xml_node_sensor_base.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------*/


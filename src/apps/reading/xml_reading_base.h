/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_XML_READING_BASE_H
#define __SHAWN_APPS_XML_READING_BASE_H

#include "_apps_enable_cmake.h"
#ifdef	ENABLE_READING

//#define MAX_DOUBLE 1E+300 	// Definition for infinite double value
#define TRACE_READINGS(x) //x			//show all reading maps

#include "apps/reading/xml_readings_specifications.h"
#include "sys/simulation/simulation_controller.h"

#include <limits>

namespace xmlreading
{
	class XMLReadingBase 
	{
	public:
		double current_time_;
	public:
		XMLReadingBase();
		//-----------------------------------------------------------------------
		virtual ~XMLReadingBase();
		//-----------------------------------------------------------------------
		virtual void notify_reading_changed_handlers() throw() = 0;
		//-----------------------------------------------------------------------
		virtual void refresh_value_durations() throw() = 0;
		//-----------------------------------------------------------------------
		virtual void receive_and_pass_settings(xmlreading::Settings&, std::string&) throw() = 0;
		//-----------------------------------------------------------------------
		virtual void receiving_new_values(xmlreading::Readings&) throw() = 0;
		//-----------------------------------------------------------------------
		virtual void set_next_event() throw() = 0;
		//-----------------------------------------------------------------------
		virtual void prepare_parsing(shawn::SimulationController&) throw() = 0;
		//-----------------------------------------------------------------------	
	};
}

#endif 
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/reading/xml_reading_base.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------*/


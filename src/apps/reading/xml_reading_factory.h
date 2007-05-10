/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_XML_READING_FACTORY_H
#define __SHAWN_APPS_XML_READING_FACTORY_H
 
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "sys/simulation/simulation_controller.h"
#include "apps/reading/xml_reading.h"
#include "sys/util/defutils.h"

namespace xmlreading
{	
	class XMLReadingFactory
	{
	public:
		XMLReadingFactory();
		//----------------------------------------------------------------------
		~XMLReadingFactory();
		//----------------------------------------------------------------------
		void build_appropriate_reading(shawn::SimulationController&, std::string&, std::string&, std::string&) throw();
		//----------------------------------------------------------------------
	};
}
 
#endif
#endif

 /*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/reading/xml_reading_factory.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------*/


/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "apps/reading/xml_reading_factory.h"

namespace xmlreading
{
	XMLReadingFactory::XMLReadingFactory(){}
	//----------------------------------------------------------------------
	XMLReadingFactory::~XMLReadingFactory(){}
	//----------------------------------------------------------------------
	void XMLReadingFactory::build_appropriate_reading(shawn::SimulationController& sc, std::string& type, std::string& data, std::string& reading_name_) throw()
	{
		TRACE("XML_READING_FACTORY: building appropriate reading");
		if(type == "simple")
		{
			if(data == "int")
			{
				TRACE("XMLREADINGFACTORY: building and registering XMLIntegerReading");
				sc.reading_keeper_w().add( new XMLIntegerReading(reading_name_));
			}else if(data == "double")
			{
				TRACE("XMLREADINGFACTORY: building and registering XMLDoubleReading");
				sc.reading_keeper_w().add( new XMLDoubleReading(reading_name_));
			}else if(data =="bool")
			{
				TRACE("XMLREADINGFACTORY: building and registering XMLBoolReading");
				sc.reading_keeper_w().add( new XMLBoolReading(reading_name_));
			}else if(data == "string")
			{
				TRACE("XMLREADINGFACTORY: building and registering XMLStringReading");
				sc.reading_keeper_w().add( new XMLStringReading(reading_name_));
			} else
			{
				TRACE("No appropriate ReadingData found!");
			}
		}else ABORT_NOT_IMPLEMENTED;
	}
} 

#endif
 
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/reading/xml_reading_factory.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------*/


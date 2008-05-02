/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "apps/reading/xml_node_sensor_factory.h"

namespace xmlreading
{
	XMLNodeSensorFactory::XMLNodeSensorFactory(){}
	//----------------------------------------------------------------------
	XMLNodeSensorFactory::~XMLNodeSensorFactory(){}
	//----------------------------------------------------------------------
	void XMLNodeSensorFactory::build_appropriate_sensor(shawn::SimulationController& sc,shawn::Node& n, std::string& type, std::string& data, std::string& reading_name_) throw()
	{
		READING_TRACE("XML_SENSOR_READING_FACTORY: build approproiate sensor");
		if(type == "simple")
		{
			if(data == "int")
			{
				READING_TRACE("XML_SENSOR_READING_FACTORY: building and registering XMLNodeIntegerSensor");
				sc.sensor_keeper_w().add( new XMLNodeIntegerSensor(n, reading_name_));
			}else if(data == "double")
			{
				READING_TRACE("XML_SENSOR_READING_FACTORY: building and registering XMLNodeDoubleSensor");
				sc.sensor_keeper_w().add( new XMLNodeDoubleSensor(n, reading_name_));
			}else if(data =="bool")
			{
				READING_TRACE("XML_SENSOR_READING_FACTORY: building and registering XMLNodeBoolSensor");
				sc.sensor_keeper_w().add( new XMLNodeBoolSensor(n, reading_name_));
			}else if(data == "string")
			{
				READING_TRACE("XML_SENSOR_READING_FACTORY: building and registering XMLNodeStringSensor");
				sc.sensor_keeper_w().add( new XMLNodeStringSensor(n, reading_name_));
			} else {
				READING_TRACE("No appropriate NodeReadingData found!");
			}
		}else ABORT_NOT_IMPLEMENTED;
	}
} 

#endif
 
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/reading/xml_node_sensor_factory.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------*/


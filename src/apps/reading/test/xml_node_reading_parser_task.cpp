/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_READING

#include "apps/reading/test/xml_node_reading_parser_task.h" 
#include "apps/reading/xml_node_sensor.h"
#include "sys/node.h"
#include "sys/world.h"

namespace reading
{
	//--------------------------------------------------------------
	XMLNodeReadingParserTask::XMLNodeReadingParserTask(){}
	//--------------------------------------------------------------
	XMLNodeReadingParserTask::~XMLNodeReadingParserTask(){}
	//--------------------------------------------------------------
	std::string XMLNodeReadingParserTask::name(void) const throw()
	{
		return "xml_node_reading_parser_task";
	}
	//--------------------------------------------------------------
	std::string XMLNodeReadingParserTask::description(void) const throw()
	{
		return "XML node reading parser task. It should read an XML file with information relating to nodes. The parsed information provides a sensor which holds the information about the nodes currently available in the world.";
	}
	//--------------------------------------------------------------
	void XMLNodeReadingParserTask::run(shawn::SimulationController& sc)
	throw (std::runtime_error)
	{
		require_world(sc);
		
		reading_file_ = sc.environment().required_string_param("reading_file");
		
		reading_name_ = sc.environment().required_string_param("name");
		
		set_document_uri(&reading_file_ [0]);
		
		parse();
		
		world_ = &sc.world_w();
		
		assert(world_->node_count()>0);
		
		shawn::Node& n = *(world_->begin_nodes_w());
		
		ns_fac = new xmlreading::XMLNodeSensorFactory();
		
		ns_fac->build_appropriate_sensor(sc, n, type, data, reading_name_);
		
		reading::SensorKeeper::Handle sh = sc.sensor_keeper_w().find_w("Pulswerte");
		
		xmlreading::XMLNodeIntegerSensor* xmlnir = dynamic_cast<xmlreading::XMLNodeIntegerSensor*>(sh.get());
		
		xmlnir->prepare_parsing(sc);
		
		xmlnir = NULL;
		ns_fac = NULL;
		delete xmlnir;
		delete ns_fac;
	}
	//--------------------------------------------------------------
	void XMLNodeReadingParserTask::start_element(const std::string& name, const shawn::xml::SAXInterruptibleReader::AttList attList) throw(std::runtime_error)
	{
		attList_ = attList;
		if("reading" == name)
		{
			AttList::iterator pos = attList_.find("type");
			type = pos->second;
			pos = attList_.find("data");
			data = pos->second;
		}
	}
	//--------------------------------------------------------------
	void XMLNodeReadingParserTask::end_element(std::string name) throw(std::runtime_error)
	{
		if("setting" == name)
		{
			interrupt();
		}
	}
	//--------------------------------------------------------------
}

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/test/xml_node_reading_parser_task.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------*/
 

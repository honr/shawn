/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_READING

#include "apps/reading/test/xml_reading_parser_task.h"

namespace reading
{
	//--------------------------------------------------------------
	XMLReadingParserTask::XMLReadingParserTask(){}
	//--------------------------------------------------------------
	XMLReadingParserTask::~XMLReadingParserTask()
	{
		sc_ = NULL;
		world_ = NULL;
		delete sc_;
		delete world_;
	}
	//--------------------------------------------------------------
	std::string XMLReadingParserTask::name(void) const throw()
	{
		return "xml_reading_parser_task";
	}
	//--------------------------------------------------------------
	std::string XMLReadingParserTask::description(void) const throw()
	{
		return "Reading parser task. It should read an XML file and display the information in the console, when running shawn.This is all done by creating a Reading. It should create an ReadingParser and the parsed information should be passed to the Reading";
	}
	//--------------------------------------------------------------
	void XMLReadingParserTask::run(shawn::SimulationController& sc)
	throw (std::runtime_error)
	{
		require_world(sc);
		// Sets the relative path to the XML file which has to be parsed
		reading_file_ = sc.environment().required_string_param("reading_file");
		reading_name_ = sc.environment().required_string_param("name");
		// Tell the parser where to find the XML file
		set_document_uri(&reading_file_ [0]);
		// Parse 
		parse();
		
		TRACE("XMLREADINGPARSERTASK: creating reading factory obj");
		// Build correct reading and register the Object in the reading keeper
		xmlreading::XMLReadingFactory* xmlreadingfactory_ = new xmlreading::XMLReadingFactory();
		xmlreadingfactory_->build_appropriate_reading(sc, type, data, reading_name_);
		// Get reading handle. If you do not know, what kind of reading you have build, have a look at youre
		// XML file under the <reading>-tag. e.G.'data="int"' stands for a XMLIntegerReading.
 		// Therefore type in the name of the reading, for our example: "xml_integer_reading".	
		reading::ReadingKeeper::Handle rh = sc.reading_keeper_w().find_w("Temperaturwerte");
		// Cast handle to reading 
		xmlreading::XMLIntegerReading* xml_ir = dynamic_cast<xmlreading::XMLIntegerReading*>(rh.get());
		world_ = &sc.world_w();
		// Check, if world has nodes 
		assert(world_->node_count()>0);
	 	
		//Granularität
		shawn::Vec* gran = new shawn::Vec(2,2,2);
		//posVec
		int x = 0;
		int y = 0;
		int z = 0;
		shawn::Vec* v = new shawn::Vec(x,y,z);
		
		for(shawn::World::node_iterator it = world_->begin_nodes_w(); it!=world_->end_nodes_w(); ++it)
		{
			*v = shawn::Vec(x,y,z);
			shawn::Node& n = *it;
			n.set_real_position(*v);
			xmlreading::XMLIntegerSensor* is_ = new xmlreading::XMLIntegerSensor(n, *xml_ir, *gran);
			x+=2;
		}
		
		
		
		//Original stuff
		// Get a node 
		//shawn::Node& n = *(world_->begin_nodes_w());
		// Create Sensor relating to the XML reading file 
		//xmlreading::XMLIntegerSensor* is_ = new xmlreading::XMLIntegerSensor(n, *xml_ir);
		// prepare the actual parsing process
		xml_ir->prepare_parsing(sc);
		
		xmlreadingfactory_ = NULL;
		delete xmlreadingfactory_;
		//is_ = NULL;
		//delete is_;	
	}
	//--------------------------------------------------------------
	void XMLReadingParserTask::start_element(const std::string& name, const shawn::xml::SAXInterruptibleReader::AttList attList) throw(std::runtime_error)
	{
		TRACE("XMLREADINGPARSERTASK: parsing reading tag");
		attList_ = attList;
		if("reading" == name)
		{
			TRACE("XMLREADINGPARSERTASK: get type");
			AttList::iterator pos = attList_.find("type");
			type = pos->second;
			TRACE("XMLREADINGPARSERTASK: get data");
			pos = attList_.find("data");
			data = pos->second;
			TRACE("XMLREADINGPARSERTASK: done");
		}
		if("settings" == name)
			interrupt();
	}
	//--------------------------------------------------------------
	void XMLReadingParserTask::end_element(std::string name) throw(std::runtime_error)
	{
	}
	//--------------------------------------------------------------
}

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/test/xml_reading_parser_task.cpp,v $
 * Version $Revision: 1.6 $
 * Date    $Date: 2007/01/16 16:24:02 $
 *-----------------------------------------------------------------------*/
 

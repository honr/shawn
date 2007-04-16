/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "apps/reading/xml_node_reading_parser.h"

#include <string>

namespace xmlreading
{
	// ----------------------------------------------------------------------
	XMLNodeReadingParser::XMLNodeReadingParser(xmlreading::XMLNodeSensorBase& node_sensor) : node_sensor_ (&node_sensor)
	{
		parser_readings_ = new NodeReadings();
		settings_ = new Settings();
	}
	// ----------------------------------------------------------------------
	XMLNodeReadingParser::~XMLNodeReadingParser() 
	{
		parser_readings_ = NULL;
		settings_ = NULL;
		delete parser_readings_;
		delete settings_;
	}
	// ----------------------------------------------------------------------
	void XMLNodeReadingParser::start_element(const std::string& name, const AttList attList) throw(runtime_error)
	{
		TRACE_PARSER("XML_NODE_READING_PARSER");
		attList_ = attList;
		
		if(!reading_initialized_)
		{	
			if("reading" == name)
			{
				TRACE_PARSER("	ReadingData parsed");
				ReadingData* rd = new ReadingData;
				AttList::iterator pos = attList_.find("type");
				rd->type = pos->second;
				pos = attList_.find("data");
				rd->data = pos->second;
				reading_data_type_ = pos->second;
				reading_initialized_ = true;
			}
		}
		if(!settings_parsed_)
		{
			if("setting" == name)
			{
				TRACE_PARSER("	SettingData parsed");
				SettingData* sd = new SettingData; 
				AttList::iterator pos =attList_.find("name");
				sd->name = pos->second;
				pos = attList_.find("value");
				sd->value = pos->second;
				settings_->push_back(sd);
				node_sensor_->receive_settings(*settings_, reading_data_type_);
				settings_parsed_ = true;
			}
		}
		if("time" == name)
		{
			TRACE_PARSER("	time tag parsed");
			AttList::iterator pos = attList_.find("t_val");
			if( pos != attList.end() ) 
			{
				t_val  = shawn::conv_string_to_double(pos->second);
				set_next_parse_event_time();
			}
		}
		if("node" == name)
		{
			TRACE_PARSER("	node tag parsed");
			AttList::iterator pos = attList_.find("id");
			int id = shawn::conv_string_to_int(pos->second);
			pos = attList_.find("val");
			std::string val = pos->second;
			pos = attList_.find("duration");
			double e = shawn::conv_string_to_double(pos->second);
			Values value = std::pair<string, double>(val, e); 
			insert_node_reading(id, value);
		}
	}
	// ----------------------------------------------------------------------
	void XMLNodeReadingParser::end_element(std::string name) throw(runtime_error)
	{
		if("time" == name)
		{
			TRACE_PARSER("XML_NODE_REAIDNG_PARSER:	closing time tag parsed");
			if( t_val > node_sensor_->current_time_ )
				{
					interrupt();
					node_sensor_->set_next_event();
				}
		}
		if("reading" == name)
		{
			TRACE_PARSER("XML_NODE_READING_PARSER: closing reading tag parsed");
			parsing_done_ = true;
			node_sensor_->set_next_event();
		}
	}
	// ----------------------------------------------------------------------
	void XMLNodeReadingParser::insert_node_reading(int& id, xmlreading::Values& value) throw()
	{
		TRACE_PARSER("XML_NODE_READING_PARSER:	insert node reading");
		pair<int, xmlreading::Values> new_reading (id, value);
		TRACE_PARSER("	to be inserted: id: "<<id<<" val: "<<value.first<<" dur: "<<value.second);
		parser_readings_->insert(new_reading);
		for(xmlreading::NodeReadings::iterator it = parser_readings_->begin(); it!= parser_readings_->end(); ++it)
		{
			if(it->first == id)
			{
				if(it->second.first != value.first || it->second.second != value.second)
				{	
					parser_readings_->erase(it);
					parser_readings_->insert(new_reading);
					break;
				}
			}  
		}
	}
	// ----------------------------------------------------------------------
	void XMLNodeReadingParser::set_next_parse_event_time() throw()
	{
		TRACE_PARSER("XML_NODE_READING_PARSER: set next parse event time");
		next_parse_event_time_ = t_val;
	}
	// ----------------------------------------------------------------------
	void XMLNodeReadingParser::timeout(shawn::EventScheduler& event_scheduler, shawn::EventScheduler::EventHandle event_handle, double time, shawn::EventScheduler::EventTagHandle& event_tag_handle) throw()
	{
		TRACE_PARSER("XML_NODE_READING_PARSER: parse event at: "<<next_parse_event_time_);
		node_sensor_->current_time_ = next_parse_event_time_;
		TRACE_READINGS(show_parser_readings());
		node_sensor_->receive_new_values(*parser_readings_);
		parse();
	}
	// ----------------------------------------------------------------------
	void XMLNodeReadingParser::show_parser_readings() throw()
	{
		TRACE_PARSER("XML_NODE_READING_PARSER: show node readings");
		for(NodeReadings::iterator it = parser_readings_->begin(); it!= parser_readings_->end(); ++it)
		{
			std::cout<<"node_id: "<<it->first<<" val: "<<it->second.first<<" duration: "<<it->second.second<<std::endl;
		}
	}
}

#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/reading/xml_node_reading_parser.cpp,v $
* Version $Revision: 1.3 $
* Date    $Date: 2006/04/13 17:30:35 $
*-----------------------------------------------------------------------*/


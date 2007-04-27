/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "apps/reading/xml_reading_parser.h"

namespace xmlreading
{
	// ----------------------------------------------------------------------
	XMLReadingParser::XMLReadingParser(XMLReadingBase& xmlreadingbase) : xmlreadingbase_ (&xmlreadingbase)
	{
		parserreadings_ = new Readings();
		settings_ = new Settings();
		parsing_done_ = false;	
		settings_parsed_ = false;
	}
	// ----------------------------------------------------------------------
	XMLReadingParser::~XMLReadingParser() 
	{
		delete parserreadings_;
		delete settings_;
		delete xmlreadingbase_;
		parserreadings_ = NULL;
		settings_ = NULL;
		xmlreadingbase_ = NULL;
	}
	// ----------------------------------------------------------------------
	void XMLReadingParser::start_element(const std::string& name, const AttList attList) throw(runtime_error)
	{
		if(!reading_initialized_)
		{	
			if("reading" == name)
			{
				TRACE_PARSER("XMLREADINGPARSER: parsing reading data");
				ReadingData* rd = new ReadingData;
				AttList::const_iterator pos = attList.find("type");
				rd->type = pos->second;
				pos = attList.find("data");
				rd->data = pos->second;
				reading_data_type_ = pos->second;
			}
			reading_initialized_ = true;
		}else if(!settings_parsed_ && "setting" == name)
		{
			TRACE_PARSER("XMLREADINGPARSER: parsing setting data");
			SettingData* sd = new SettingData; 
			AttList::const_iterator pos =attList.find("name");
			sd->name = pos->second;
			pos = attList.find("value");
			sd->value = pos->second;
			settings_->push_back(sd);
			xmlreadingbase_->receive_and_pass_settings(*settings_, reading_data_type_);
			settings_parsed_ = true;
		}else if("time" == name)
		{
			TRACE_PARSER("XMLREADINGPARSER: parsing time tag");
			AttList::const_iterator pos = attList.find("t_val");
			if( pos != attList.end() ) 
			{
				t_val  = shawn::conv_string_to_double(pos->second);
				set_next_parse_event_time();
			}
		}else if("value" == name)
		{
			TRACE_PARSER("XMLREADINGPARSER: parsing value tag");
			AttList::const_iterator pos = attList.find("pos_x");
			double x = shawn::conv_string_to_double(pos->second);
			
			pos = attList.find("pos_y");
			double y = shawn::conv_string_to_double(pos->second);
			
			pos = attList.find("pos_z");
			double z = shawn::conv_string_to_double(pos->second);
			
			shawn::Vec v = shawn::Vec(x,y,z);	
			pos = attList.find("val");
			string s = pos->second;
			pos = attList.find("duration");
			double e = shawn::conv_string_to_double(pos->second);
			
			Values val = std::pair<string, double>(s, e); 
			
			update_readings(v, val);
		}
		
	}
	// ----------------------------------------------------------------------
	void XMLReadingParser::end_element(std::string name) throw(runtime_error)
	{
		if("time" == name)
		{
			TRACE_PARSER("XMLREADINGPARSER: parsing end time tag");
			if( t_val >= xmlreadingbase_->current_time_ )
			{
				interrupt();
				xmlreadingbase_->set_next_event();
				
			}
		}
		else if("reading" == name)
		{
			TRACE_PARSER("XMLREADINGPARSER: parsing end reading tag");
			parsing_done_ = true;
			xmlreadingbase_->set_next_event();
		}
	}
	// ----------------------------------------------------------------------
	void XMLReadingParser::update_readings(shawn::Vec v, Values values) throw()
	{
		TRACE_PARSER("XMLREADINGPARSER: update parser readings");
		pair<shawn::Vec, Values> vec (v, values);
		parserreadings_->insert(vec);
		
		for(Readings::iterator it = parserreadings_->begin(); it!= parserreadings_->end(); ++it)
		{
			if(it->first == v)
			{
				if(it->second.first != values.first || it->second.second != values.second)
				{	
					parserreadings_->erase(it);
					parserreadings_->insert(vec);
					break;
				}
			}  
		}
	}
	// ----------------------------------------------------------------------
	void XMLReadingParser::set_next_parse_event_time() throw()
	{
		TRACE_PARSER("XMLREADINGPARSER: set next parse event time: ");
		next_parse_event_time_ = t_val;
	}
	// ----------------------------------------------------------------------
	void XMLReadingParser::timeout(shawn::EventScheduler& event_scheduler, shawn::EventScheduler::EventHandle event_handle, double time, shawn::EventScheduler::EventTagHandle& event_tag_handle) throw()
	{
		TRACE_PARSER("XMLREADINGPARSER: parse event at: "<<next_parse_event_time_);
		
		TRACE_READINGS(show_parser_readings());
		
		xmlreadingbase_->current_time_ = next_parse_event_time_;
		xmlreadingbase_->receiving_new_values(*parserreadings_);
 		parse();
	}
	// ----------------------------------------------------------------------
	void XMLReadingParser::show_parser_readings() throw()
	{
		TRACE_PARSER("XMLREADINGPARSER: showing parser readings");
		for(Readings::iterator it = parserreadings_->begin(); it!= parserreadings_->end(); ++it)
		{
			std::cout<<"	"<<it->first.x()<<" "<<it->first.y()<<" "<<it->first.z()<<"   "<<it->second.first<<" "<<it->second.second<<std::endl;
		}
	}
}

#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/reading/xml_reading_parser.cpp,v $
* Version $Revision: 1.5 $
* Date    $Date: 2006/10/10 14:11:05 $
*-----------------------------------------------------------------------*/


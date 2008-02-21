/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_XML_NODE_SENSOR_H
#define __SHAWN_APPS_XML_NODE_SENSOR_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "sys/simulation/simulation_controller.h"
#include "apps/reading/xml_node_reading_parser.h"
#include "apps/reading/xml_node_sensor_base.h"
#include "apps/reading/node_sensor.h"
#include "apps/reading/sensor_keeper.h"
#include "sys/event_scheduler.h"
#include "sys/world.h"
#include "sys/node.h"
#include "sys/vec.h"
#include <map>

namespace xmlreading
{
	template <class T>
	class XMLNodeSensor 
		: public reading::NodeSensor<T>, 
		  public xmlreading::XMLNodeSensorBase, 
		  public shawn::EventScheduler::EventHandler 
	{
	private:
		shawn::World* world_;
		///
		bool nschl_not_empty;
		///
		double last_event_time_;
		///
		double past_world_time_;
		///
		std::string reading_file_;
		///
		std::string reading_data_type_;
		///
		double next_invalidation_event_time_;
		///
		xmlreading::Settings* sensor_settings_;
		///
		xmlreading::NodeReadings* sensor_readings_;
		///
		xmlreading::NodeReadings* parser_readings_;
		///
		shawn::EventScheduler::EventHandle event_handle_;
		///
		xmlreading::XMLNodeReadingParser* node_reading_parser_;
		///
	public:
		typedef T ValueType;
		//
		///@name constructions / destruction
		///@{
		XMLNodeSensor(shawn::Node& n) : reading::NodeSensor<T>(n)
		{
			current_time_ = 0;
			sensor_settings_ = new xmlreading::Settings;
			sensor_readings_ = new xmlreading::NodeReadings;
			parser_readings_ = new xmlreading::NodeReadings;
		}
		///
		virtual ~XMLNodeSensor()
		{
			sensor_settings_ = NULL;
			sensor_readings_ = NULL;
			parser_readings_ = NULL;
			delete sensor_settings_;
			delete sensor_readings_;
			delete parser_readings_;
		}
		///@}
		///
		///@name class own methods
		///@{
		/** Prepares the actual parsing process */
		void prepare_parsing(shawn::SimulationController& sc) throw()
		{
			TRACE("XML_NODE_SENSOR: prepare parsing process");
			world_ = &(sc.world_w());
			/** Sets the relative path to the XML file which has to be parsed */
			reading_file_ = sc.environment().required_string_param("reading_file");
			/** Instantiate ReadingParser with current type of XMLReading<T> */
			node_reading_parser_ = new XMLNodeReadingParser(*this);
			/** Tell the parser where to find the XML file */
			node_reading_parser_->set_document_uri(&reading_file_ [0]);
			/** Parse the given file*/
			node_reading_parser_->parse();
		}
		///
		virtual void receive_settings(xmlreading::Settings& settings, std::string& reading_data_type) throw()
		{
			TRACE("XML_NODE_SENSOR: receive settings");
			sensor_settings_ = &settings;
			reading_data_type_ = reading_data_type;
		}
		///
		/** When the parser delivers new values they need to be inserted into the local Node_Readings varaible (xmlreadings) and erased out of the Node_Readings variable parserreadings in order to prepare the next event. */ 
		virtual void receive_new_values(xmlreading::NodeReadings& parser_node_readings) throw()
		{	
			TRACE("XML_NODE_SENSOR: receiving new values");
			parser_readings_ = &parser_node_readings;
			refresh_value_durations();
			for(NodeReadings::iterator pnr = parser_readings_->begin(); pnr!= parser_readings_->end(); ++pnr)
			{
				int i = sensor_readings_->size();
				sensor_readings_->insert(*pnr);
				int j = sensor_readings_->size();
								
				if( i == j )
				{
					for(NodeReadings::iterator snr= sensor_readings_->begin(); snr!=sensor_readings_->end(); ++snr)
					{
						if(pnr->first == snr->first)
						{
							if(pnr->second.first != snr->second.first)
							{
								sensor_readings_->erase(snr);
								lookup_node_sensor_listener(pnr);
								sensor_readings_->insert(*pnr);
							}
							if(pnr->second.second != snr->second.second)
							{
								sensor_readings_->erase(snr);
								sensor_readings_->insert(*pnr);
							}	
						}	
					}
				}else
				{
					lookup_node_sensor_listener(pnr);
				}
				parser_readings_->erase(pnr);
			}
			TRACE_READINGS(show_sensor_readings());
		}
		///
		/** This method erases all node reading values whose duration is null */
		virtual void erase_sensor_readings() throw()
		{
			refresh_value_durations();
			for(NodeReadings::iterator nr = sensor_readings_->begin(); nr != sensor_readings_->end(); ++nr)
			{
				if(nr->second.second <= 0)
				{
					lookup_node_sensor_listener(nr);
					sensor_readings_->erase(nr);
					if(!sensor_readings_->size() == 0)
						--nr;
				}
			
			}
			TRACE_READINGS(show_sensor_readings());
		}
		///
		/** Sets the next event in relation to the next parse event of the parser and the next invalidation event of the reading*/
		virtual void set_next_event() throw()
		{		
			TRACE("XML_NODE_SENSOR: set next event");
			set_next_invalidation_event_time();
	
			node_reading_parser_->set_next_parse_event_time();
			if(sensor_readings_->size() == 0 && !node_reading_parser_->parsing_done_)
			{
				event_handle_ = world_->scheduler_w().new_event(*node_reading_parser_, 
				node_reading_parser_->get_next_parse_event_time(), NULL);
				
			}else if(sensor_readings_->size() != 0 && node_reading_parser_->parsing_done_)
			{
				event_handle_ = world_->scheduler_w().new_event(*this, next_invalidation_event_time_, NULL);	
			
			}else if(sensor_readings_->size() == 0 && node_reading_parser_->parsing_done_)
			{
				std::cout<<"Done parsing & passing values"<<std::endl;
								
			}else if(sensor_readings_->size() !=0 && next_invalidation_event_time_ <= node_reading_parser_->get_next_parse_event_time())
			{	
				event_handle_ = world_->scheduler_w().new_event(*this, next_invalidation_event_time_, NULL);
				
			}else if(sensor_readings_->size() !=0 && next_invalidation_event_time_ > node_reading_parser_->get_next_parse_event_time())
			{
				event_handle_ = world_->scheduler_w().new_event(*node_reading_parser_, node_reading_parser_->get_next_parse_event_time(), NULL);
			}
		}
		///@}
		///@name event handling
		///@{
		virtual void timeout(shawn::EventScheduler& event_scheduler, shawn::EventScheduler::EventHandle event_handle, double time, shawn::EventScheduler::EventTagHandle& event_tag_handle) throw()
		{
			TRACE("XML_NODE_SENSOR: invalidation event at: "<<next_invalidation_event_time_);
			current_time_ = next_invalidation_event_time_;
			erase_sensor_readings();
			set_next_event();
		}
		///@}
		///@name data access
		///@{
		virtual const NodeReadings& get_sensor_readings() const throw()
		{
			return *sensor_readings_;
		}
		///@}
	private:
		///@name class own methods
		///@{
		void show_sensor_readings() throw()
		{
			std::cout<<"XML_NODE_SENSOR: current sensor node readings"<<std::endl;
			for(NodeReadings::iterator it = sensor_readings_->begin(); it != sensor_readings_->end(); ++it)
			{
				std::cout<<"node_id: "<<it->first<<" val: "<<it->second.first<<" dur: "<<it->second.second<<std::endl;
			}
		}
		///
		/** While simulating, time goes on. All value durations have to be updated with reference to 
		  * the actual simulation time.*/ 
		virtual void refresh_value_durations() throw()
		{	
			TRACE("XML_NODE_SENSOR: refresh value durations");
			past_world_time_ = current_time_ - last_event_time_;
	
			for(NodeReadings::iterator it = sensor_readings_->begin(); it!=sensor_readings_->end(); ++it)
			{
				it->second.second = it->second.second - past_world_time_;
				if(it->second.second < EPSILON)
					it->second.second = 0.0;
			}
			last_event_time_ = current_time_;
			TRACE_READINGS(show_sensor_readings());
		}
		///
		/** Sets the next invalidation event time. The invalidation event time is addicted to the values durations. */
		virtual void set_next_invalidation_event_time() throw()
		{
			TRACE("XML_NODE_SENSOR: set next invalidation event time");
			next_invalidation_event_time_ = INT_MAX;
			if(sensor_readings_ != NULL)
			{
				for(NodeReadings::iterator it = sensor_readings_->begin(); it!=sensor_readings_->end(); ++it)
				{
					if(it->second.second + current_time_ <= next_invalidation_event_time_)
					{
						next_invalidation_event_time_ = it->second.second + current_time_;
					}
				}
			}
		}
		///
		/** Returns the next invalidation event time. */
		virtual double get_next_invalidation_event_time() throw() { return next_invalidation_event_time_;}
		///
		/** Checks, if there are any listeners (reading changed handlers)*/
		virtual void lookup_node_sensor_listener(NodeReadings::iterator iter) throw()
		{
			TRACE("NODE_SENSOR: lookup sensor changed handlers");
			if(nschl_not_empty)
			{

#ifndef WIN32
#warning DIRTY WORKAROUND TO GET IT COMPILING WITH g++ 4.x, FIX THE LINES BELOW
#endif

				std::vector< std::pair< reading::NodeSensorChangedHandler*, shawn::Node* > >::iterator it;
				for(it = reading::NodeSensor<T>::nschl_.begin(); it != reading::NodeSensor<T>::nschl_.end(); ++it)
				{	
					if( it->second->id() == iter->first )
					{
						it->first->node_sensor_changed(*this);
					}
				}
			} else 
				TRACE("No Listeners available");
		}
		///
		/** checks if the sensor has listeners*/
		virtual void has_listeners() throw()
		{
			if(!reading::NodeSensor<T>::nschl_.empty())
				nschl_not_empty = true;
			else 
				nschl_not_empty = false;
		}
		///@}
	};
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	///
	class XMLNodeIntegerSensor 
		: public XMLNodeSensor<int>
	{
	public:	
		///@name construction / destruction
		///@{
		XMLNodeIntegerSensor(shawn::Node& n, std::string& reading_name_) : XMLNodeSensor<int>(n), reading_name (reading_name_){}
		///
		virtual ~XMLNodeIntegerSensor(){}
		///@}
		///@name keeper managed
		///@{
		virtual std::string name() const throw() {return reading_name;}
		///
		virtual std::string description() const throw() {return "A node sensor which responds to integer values";}
		///@}
		///@name data access
		///@{
		virtual ValueType value( const shawn::Node& n ) const throw()
		{
			std::string s = "";
			s = get_sensor_readings().find(n.id())->second.first;	
			return shawn::conv_string_to_int(s);
		}
		///@}
	private:
		std::string reading_name;	
	}; 
	DECLARE_HANDLES(XMLNodeIntegerSensor);
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	///
	class XMLNodeDoubleSensor 
		: public XMLNodeSensor<double>
	{
	public:
		///@name construction /destruction
		///@{
		XMLNodeDoubleSensor(shawn::Node& n, std::string& reading_name_) : XMLNodeSensor<double>(n), reading_name (reading_name_){}
		///
		virtual ~XMLNodeDoubleSensor(){}
		///@}
		///@name keeper managed
		///@{
		virtual std::string name() const throw() {return reading_name;}
		///
		virtual std::string description() const throw() {return "A node sensor which responds to integer values";}
		///@}
		///@name data access
		///@{
		virtual ValueType value( const shawn::Node& n ) const throw()
		{
			std::string s = "";
			s = get_sensor_readings().find(n.id())->second.first;	
			return shawn::conv_string_to_double(s);
		}
		///@}
	private:
		std::string reading_name;	
	}; 
	DECLARE_HANDLES(XMLNodeDoubleSensor);
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	///
	class XMLNodeBoolSensor 
		: public XMLNodeSensor<bool>
	{
	public:
		///@name construction /destruction
		///@{
		XMLNodeBoolSensor(shawn::Node& n, std::string& reading_name_) : XMLNodeSensor<bool>(n), reading_name (reading_name_){}
		///
		virtual ~XMLNodeBoolSensor(){}
		///@}
		///@name keeper managed
		///@{
		virtual std::string name() const throw() {return reading_name;}
		///
		virtual std::string description() const throw() {return "A node sensor which responds to integer values";}
		///@}
		///@name data access
		///@{
		virtual ValueType value( const shawn::Node& n ) const throw()
		{
			std::string s = "";
			s = get_sensor_readings().find(n.id())->second.first;	
			return shawn::conv_string_to_bool(s);
		}
		///@}
	private:
		std::string reading_name;	
	}; 
	DECLARE_HANDLES(XMLNodeBoolSensor);
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	///
	class XMLNodeStringSensor : public XMLNodeSensor<std::string>
	{
	public:
		///@name construction /destruction
		///@{
		XMLNodeStringSensor(shawn::Node& n, std::string& reading_name_) : XMLNodeSensor<std::string>(n), reading_name (reading_name_){}
		///
		virtual ~XMLNodeStringSensor(){}
		///@}
		///@name keeper managed
		///@{
		virtual std::string name() const throw() {return reading_name;}
		///
		virtual std::string description() const throw() {return "A node sensor which responds to integer values";}
		///@}
		///@name data access
		///@{
		virtual ValueType value( const shawn::Node& n ) const throw()
		{
			std::string s = "";
			s = get_sensor_readings().find(n.id())->second.first;	
			return s;
		}
		///@}
	private:
		std::string reading_name;	
	}; 
	DECLARE_HANDLES(XMLNodeStringSensor);
	///
};

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/reading/xml_node_sensor.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------*/


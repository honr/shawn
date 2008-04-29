/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_XML_NODE_READING_PARSER_H
#define __SHAWN_APPS_XML_NODE_READING_PARSER_H
#include "_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#ifndef TRACE_PARSER
#define TRACE_PARSER(x)  std::cout << x << std::endl;	//show comments
#endif

#include "sys/xml/tag_parser.h"
#include "sys/event_scheduler.h"
#include "sys/util/string_conv.h"
#include "sys/xml/sax_interruptible_reader.h"
#include "apps/reading/xml_node_sensor_base.h"

#include <string>

namespace xmlreading
{
	///@name data access format of the XML file is:
	///@{
	/**
	* <reading type="simple" data="int">
	*	<settings>
	*		<setting name="" value="3"/>
	*		...
	*	</settings>
	*	<readingvalues>
	*		<time t_val="0">
	*			<node id="1" val="21" duration="1.0"/>
	*			...
	*		</time>
	*		<time t_val="1">
	*			<node id="4" val="21" duration="2"/>
	*			...
	*		</time>
			...
	*	</readingvalues>
	* </reading>
	*/
	///@}
	///	XMLNodeReadingParser
	/**	This class is responsible for the actual parsing process. With it a XML file with the under the name 'data access format' given format is parsed. Furthermore all information is stored in local maps and structs. At first the SettingData is parsed. The SettingData contains information about what to do with the parsed information and is passed to the sensor immediatly. For now there are now settings for how to process with the node data. At a timeout-signal the parsed information is passed to the sensor. */
	class XMLNodeReadingParser 
		: public shawn::xml::SAXInterruptibleReader,
		  public shawn::xml::TagParser,
		  public shawn::EventScheduler::EventHandler
	{
	public:
		///
		double next_parse_event_time_;
		///
		bool parsing_done_;
	private:
		///
		double t_val;
		///
		bool settings_parsed_;
		///
		bool reading_initialized_;
		///
		std::string reading_data_type_;
		///
		xmlreading::Settings* settings_;
		///
		xmlreading::NodeReadings* parser_readings_;
		///
		xmlreading::XMLNodeSensorBase* node_sensor_;
		///
		shawn::EventScheduler::EventHandle event_handle_;
		///
		shawn::xml::AttList attList_;
		///	
	public:
		///@name construction/destruction
        	///@{
		XMLNodeReadingParser(xmlreading::XMLNodeSensorBase&);
		///
		~XMLNodeReadingParser();
        	///@}
        	///@name event handling
		///@{
		/** This method is called when an timeout signal has been set. Time tags from the XML file deliver the point in time, when the timeout method has to be called.
		**/
		void timeout( shawn::EventScheduler&, shawn::EventScheduler::EventHandle, double, shawn::EventScheduler::EventTagHandle& ) throw();
		///@}
		///@name class own methods
		///@{
		void insert_node_reading(int&, xmlreading::Values&) throw();
		///
		/** Set the next event time where the parser should go on with parsing */
		void set_next_parse_event_time() throw();
		///
		/** Returns the parse event time */
		double get_next_parse_event_time() throw() { return next_parse_event_time_; }
		///
		/** show the current content of the parser readings and the values */
		void show_parser_readings() throw();
		///@}
		///
	protected:
		///@name sax parser methods
		/// Callback handler for Expat opening tag events. Real handling is delegated to parse().
		void start_element(std::string, shawn::xml::AttList&) throw(std::runtime_error);
		///
		/// Callback handler for Expat closing tag events. Real handling is delegated to parse().
		void end_element(std::string) throw(std::runtime_error);
		///@}
	};
};

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/reading/xml_node_reading_parser.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------*/


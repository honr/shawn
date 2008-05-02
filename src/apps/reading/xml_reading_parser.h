/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_XML_READING_PARSER_H
#define __SHAWN_APPS_XML_READING_PARSER_H

#include "_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "apps/reading/xml_readings_specifications.h"
#include "sys/xml/sax_interruptible_reader.h"
#include "apps/reading/xml_reading_base.h"
#include "sys/util/string_conv.h"
#include "sys/event_scheduler.h"
#include "sys/xml/tag_parser.h"

namespace xmlreading
{   
	///XMLReadingParser parses a given XML file 
	/** The XMLReadingParser parses as allready mentioned a given XML file with the down listed data acces format.
	  * In doing so, the values read are stored in variables. As there are ReadingData, SettingsData and Readings.
	  * ReadingsData comprehend data as what type of reading should be created. SettingsData contain information 
	  * for the "listening" sensors about what to do with the information (values) held by a variable of the type Readings.
	  * The variable of the type Readings stores all new parsed values. 
	  * On a timeout signal all new read values are inserted into the XMLReading's variable of the type Readings.*/
	///@name data access format of the XML file is:
	/**
	* <reading type="simple" data="int">
	*	<settings>
	*		<setting name="interpolation" value="3"/>
	*		...
	*	</settings>
	*	<readingvalues>
	*		<time t_val="0">
	*			<value pos_x="1" pos_y="0.3" pos_z="17" val="21" duration="1.0"/>
	*			...
	*		</time>
	*		<time t_val="1">
	*			<value pos_x="1.53" pos_y="23" pos_z="17" val="21" duration="1"/>
	*			...
	*		</time>
			...
	*	</readingvalues>
	* </reading>
	*/
	class XMLReadingParser :
		public shawn::xml::SAXInterruptibleReader,
		public shawn::xml::TagParser,
		public shawn::EventScheduler::EventHandler
	{
	public:
		bool parsing_done_;
		double next_parse_event_time_;
	private:
		std::string reading_data_type_;
		double t_val;
		bool settings_parsed_;
		bool reading_initialized_;
		xmlreading::Settings* settings_;
		xmlreading::Readings* parserreadings_;
		xmlreading::XMLReadingBase* xmlreadingbase_;
		shawn::EventScheduler::EventHandle event_handle_;

	public:
		///@name construction/destruction
        	///@{
		XMLReadingParser(XMLReadingBase&);
		///
		~XMLReadingParser();
        	///@}
        	///@name event handling
		///@{
		
		/** This method is called when an timeout signal has been set. Time tags from the XML file deliver the point in time, when the timeout method has to be called.
		**/
		void timeout( shawn::EventScheduler&, shawn::EventScheduler::EventHandle, double, shawn::EventScheduler::EventTagHandle& ) throw();
		///@}

		/** Insert the just read values into the local Readings variable*/
		void update_readings(shawn::Vec, Values) throw();

		/** Set the next event time where the parser should go on with parsing */
		void set_next_parse_event_time() throw();

		/** Returns the parse event time */
		double get_next_parse_event_time() throw() { return next_parse_event_time_; }
		
		/** Show all values currently held by the Readings variable*/
		void show_parser_readings() throw();

	protected:
		/// Callback handler for Expat opening tag events. Real handling is delegated to parse().
		virtual void start_element(std::string, shawn::xml::AttList&) throw(std::runtime_error);
		
		/// Callback handler for Expat closing tag events. Real handling is delegated to parse().
		virtual void end_element(std::string name) throw(std::runtime_error);
	};
};

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/reading/xml_reading_parser.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------*/


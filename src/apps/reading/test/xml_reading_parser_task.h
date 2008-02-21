/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#ifndef __SHAWN_APPS_XML_READING_PARSER_TASK_H
#define __SHAWN_APPS_XML_READING_PARSER_TASK_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_READING

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "apps/reading/xml_reading_factory.h"
#include "apps/reading/xml_reading.h"
#include "apps/reading/xml_sensor.h"
#include "apps/reading/reading.h"
#include "sys/xml/sax_interruptible_reader.h"
#include "sys/event_scheduler.h"
#include "sys/node.h"
#include "sys/world.h"

#include "sys/misc/box.h"
#include "sys/util/defutils.h"

namespace reading 
{
 	class XMLReadingFactory;
	
	/// Task that runs the ReadingParser
	/**	ReadingParserTask is inherited from the superclass SimulationTask.
    	*	It contains basic methods as name() and description() and typical 
    	*	for "tasks" a run-method.
	*	If you want to run this task, you will have to create a XML file following the rules mentioned in the xml_reading_parser.h file.
    	*/
	class XMLReadingParserTask : 
		public shawn::SimulationTask, 
		public shawn::xml::SAXInterruptibleReader 
	{
	private:
		std::string reading_file_;
		std::string reading_name_;
		std::string type;
		std::string data;
		shawn::xml::AttList attList_;
		shawn::World* world_;
		shawn::SimulationController* sc_;
	public:
		// ----------------------------------------------------------------------
		///@name construction/destruction
        	///@{
        	XMLReadingParserTask();
        	///
        	virtual ~XMLReadingParserTask();
        	///@}	
        	///@name basic methods inherited from SimulationTask
        	///@{
        	/**	With this method a name is given to the task. */
        	virtual std::string name( void ) const throw();
        	///
        	/**	With this method a description is given to the task. */	
        	virtual std::string description( void ) const throw();
        	///
        	/**	This method can differ from other implemented tasks.
       	 	*	Basically it is used to run a task. 
        	*/
        	virtual void run( shawn::SimulationController& sc ) throw( std::runtime_error );
        	///@}
	protected:
		///@name basic methods inherited from SAXReader
		///@{
		/// Callback handler for Expat opening tag events. Real handling is delegated to parse().
		virtual void start_element(const std::string, shawn::xml::AttList) throw(std::runtime_error);
		///
		/// Callback handler for Expat closing tag events. Real handling is delegated to parse().
		virtual void end_element(std::string) throw(std::runtime_error);
		///@}	
	};
};

#endif
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/test/xml_reading_parser_task.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------*/


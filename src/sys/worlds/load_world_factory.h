/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_WORLDS_LOAD_WORLD_FACTORY_H
#define __SHAWN_SYS_WORLDS_LOAD_WORLD_FACTORY_H

#include "shawn_config.h"
#ifdef HAVE_EXPAT 

#include "sys/worlds/processor_world_factory.h"
#include "sys/xml/sax_reader.h"
#include "sys/xml/tag_parser.h"
#include "sys/tag.h"
#include "sys/taggings/tag_factory.h"
#include "sys/world.h"

#include <expat.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <set>
#include <exception>
#include <string>
#include <map>

namespace shawn 
{
	class TagFactoryKeeper;
	class SimulationController;

	///Creates nodes in a world by reading node information from an XML File.
	/** Currently honors location and tag information of nodes and reads the tags for the
	* world. The XML file is NOT checked for errors or schema conformance. Only one snapshot 
	* is currently read from the file. Set the desired snapshot time using set_snapshot_id().
	*
	* The expected format of the XML file is:
	* 
	* <pre>
	* <scenario>
	*   <snapshot time="0">
	*     <world>
	*       <!-- Size hint for a common communication range in the world -->
	*       <!-- This is NOT the actual communication range -->
	*       <sizehint value="12.34567" />
	*
	*       <!-- Simple value tag stored in the world -->
	*       <tag type="string" name="foo" value="bar" />
	*       ...
	*     </world>
	*     <node id="v_00_00">
	*       <!-- Node location -->
	*       <location x="00.00" y="00.00" />
	*
	*       <!-- Simple value tag -->
	*       <tag type="int" name="test_int_tag" value="123" />
	* 
	*       <!-- Map tag -->
	*       <tag type="map-string-string" name="test_map">
	*           <entry index="index1" value="value1" />
	*           <entry index="index2" value="value2" />
	*       </tag>
	* 
	*       <!-- Set tag -->
	*       <tag type="" name="">
	*           <entry value="value1" />
	*           <entry value="value2" />
	*       </tag>
	*       ...
	*     </node>
	*   </snapshot>
	*   <snapshot time="1">
	*     ...
	*   </snapshot>
	* </scenario>
	* </pre>
	*/
	class LoadWorldFactory : 
		public shawn::xml::SAXSimpleSkipReader, 
		public ProcessorWorldFactory,
		public shawn::xml::TagParser
	{

	public:
		///The current parsing state
		enum ParsingState 
		{ 
			Unknown,    ///< No expected tag has been encoutered yet
			Scenario,   ///< Inside a scenario tag
			Snapshot,   ///< Inside a snapshot tag
			Node,       ///< Inside a node tag
			World,      ///< Inside a world tag
			KiSeq,		///< Inside a tag tag named "ki_seq"
			Done        ///< Parsing has stopped
		};            

		///@name Construction / Destruction
		///@{
		LoadWorldFactory();
		virtual ~LoadWorldFactory();
		///@}

		///@name Configuration and action triggering
		///@{

		/** Set the snapshot id which will be used when reading the file */
		void set_snapshot_id( const std::string& ) throw();

		/** Actual parsing is triggered here */
		virtual void fill_world( shawn::World& ) throw();

		///@}

	protected:

		/// Callback handler for Expat opening tag events. Real handling is delegeated to parse_xml().
		virtual void handle_start_element(const char *name, const char **atts) throw(std::runtime_error);

		/// Callback handler for Expat closing tag events. Real handling is delegeated to parse_xml().
		virtual void handle_end_element(const char *name) throw(std::runtime_error);

		///Keeps the current parsing state and extracts the relevant information from the XML file.
		/** Creates Nodes in the world (Location and Tags are currently read). Also tags for the shwan::World are
		* read and attached to the world. Only called if not in skipping mode.
		*/
		void parse_xml(const char *name, const char **atts, bool opening_tag) throw(std::runtime_error);        

		///Handles &lt;scenario&gt; tags
		void handle_tag_scenario(const char **atts, bool opening_tag);        
		///Handles &lt;world&gt; tags
		void handle_tag_world(const char **atts, bool opening_tag);        
		///Handles &lt;snapshot&gt; tags
		void handle_tag_snapshot(const char **atts, bool opening_tag);        
		///Handles &lt;node&gt; tags
		void handle_tag_node(const char **atts, bool opening_tag);        
		///Handles &lt;location&gt; tags
		void handle_tag_location(const char **atts, bool opening_tag);        
		///Handles &lt;sizehint&gt; tags
		void handle_tag_sizehint(const char **atts, bool opening_tag);        

		/** Callback handler from SAXSkipReader. Invoked if the target tag in the XML file has been reached. */
		virtual void skip_target_reached(const char *name, const char **atts);

		virtual void add_node(shawn::Node*); //add nodes to world - can be rimplemented in subclasses in order to trigger the addition of nodes

	protected:
		int node_count_;						///< The number of created nodes
		shawn::World* world_;					///< Reference to the world in which we create nodes
		ParsingState parsing_state_;			///< The current parsing state
						
		SimulationController* sim_controller_;	///< Pointer to the simulation controller
	private:	
		shawn::Node* current_node_;///< The node that is currently filled with data
	};

}

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/worlds/load_world_factory.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: load_world_factory.h,v $
 *-----------------------------------------------------------------------*/

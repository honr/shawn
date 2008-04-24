/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_SYS_XML_SAX_READER_H
#define __SHAWN_SYS_XML_SAX_READER_H

extern "C" 
{
	#include "sys/xml/expat/expat.h"
}

#include <stdlib.h>
#include <set>
#include <map>
#include <deque>
#include <stdexcept>
#include <string>

using namespace std;

namespace shawn 
{
    namespace xml
    {
		typedef std::multimap<std::string, std::string> AttList;	
	
        /// Extracts the value of an attribute. Returns default_val if not existing
        std::string attribute(std::string name, AttList& atts, std::string default_val = "");
    
		
        // -------------------------------------------------------------------
        /// Simple C++ wrapper class for the SAX parser irrXML (http://www.ambiera.com/irrxml/index.html).
        /** Overwrite the handle_(start|end)_element methods to parse the file.
        *
        */
        class SAXReader 
        {
        public:
            ///@name Construction / Destruction
            ///@{

            SAXReader();
            virtual ~SAXReader();
            ///@}

            ///@name Configuration and control
            ///@{

            /// Set the xml files uri to read from. 
            /** @param s The path to the xml document. May be a relativ path. 
            */
            void set_document_uri( const std::string& s) throw();

            /// Trigger the parsing of the file
            /** Opens the file and creates the XML parser. The file is read and chunks are 
            * passed to the XML parser. The callbacks result in method calls to 
            * handle_(start|end)_element.
            * After parsing is complete, reset() is invoked
            */
            void parse() throw(std::runtime_error);

			virtual void parsing_done() throw();

            /// Reset the entire parsing state and close the current file.
            virtual void reset() throw();

            /// Stops the parsing process at the next feasible point in time
            virtual void interrupt() throw();

            ///@}

            
        protected:

            /// Empty stub
            virtual void handle_start_element(std::string name, AttList& atts) throw(std::runtime_error);
            /// Empty stub
            virtual void handle_end_element(std::string name) throw(std::runtime_error);

            bool stop_flag_;                ///< Tells the parsing process to stop at the next feasible point in time

            /// Opens the file. 
            /**
            * @throws std::runtime_error if the file could not be opened.
            */
            void open_file() throw(std::runtime_error);

            std::string document_uri_;      ///< The xml file's uri
            std::ifstream* is_;             ///< The stream to read from
            XML_Parser parser;              ///< The expat sax parser instance

            friend void saxreader_start(void *userdata, const char *name, const char **atts);
            friend void saxreader_end(void *userdata, const char *name);
        };


        // -------------------------------------------------------------------
        /// Abstract SAX Reader prepared to skip certain areas of the XML file.
        /** Implement the pure virtual methods to implement your skipping functionality.
        */
        class SAXSkipReader 
        	: public SAXReader 
        {
        public:        

            /// Reset the entire parsing state and close the current file.
            virtual void reset() throw();
        
        protected:
            ///@name SAX related stuff
            ///@{

	            ///Checks if the skip target has been reached and calls skip_target_reached if yes.
	            virtual void handle_start_element(std::string name, AttList& atts) throw(std::runtime_error);
	
	            //Checks if the skip target has been reached and calls skip_target_reached if yes.
	            virtual void handle_end_element(std::string name) throw(std::runtime_error);

            ///@}

            ///@name Skipping
            ///@{

	            ///Clear all skip targets
	            virtual void clear_skip_target() = 0;
	            
	            ///Return true if you are in skipping mode
	            virtual bool skipping() = 0;
	            
	            ///Called by SAXSkipReader handle_(start|end)_element to check whether a skip target has been reached
	            virtual bool check_skip_target_reached(std::string name, AttList& atts, bool opening_tag) = 0;
	            
	            ///Called by the SAXSkipReader once check_skip_target_reached returns true
	            virtual void skip_target_reached(std::string name, AttList& atts) = 0;

            ///@}

        };


        // -------------------------------------------------------------------
        ///Simple skipper that skips to a given tag having certain attributes and values
        /** Implement skip_target_reached to get a callback when the desired tag is encounterd.
        */
        class SAXSimpleSkipReader 
        	: public SAXSkipReader 
        {
        public:
            SAXSimpleSkipReader();

            /// Set the skip target to look for. 
            /** Set name to the name of the tag you are looking for. By default opening 
              * tags are searched.
              */
            void set_skip_target(std::string name, bool opening_tag = true);

            /// Add an attribute condition to the skipping target.
            /** All (attribute, value) pairs form an and condition.
              *
              */
            void set_skip_target_add_attr(std::string name, std::string value);

        protected:

            ///Called by the SAXSimpleSkipReader once the desired tag is encounterd.
            virtual void skip_target_reached(std::string, AttList& atts) = 0;

            ///@name Skipping
            ///@{

            ///Returns true if the skip target set by set_skip_target and set_skip_target_add_attr has been reached
            virtual bool check_skip_target_reached(string name, AttList& atts, bool opening_tag);
            ///Clears the skip target
            virtual void clear_skip_target();
            ///Returns whether we are in skipping mode
            virtual bool skipping();

            ///@}

        private:
            typedef std::map<std::string, std::string> str_str_map; ///< ///A <string, string> map
            std::string skip_to_tag_;      ///<The name of the tag we are looking for
            str_str_map skip_to_tag_atts_;  ///<Contains (name, value) to describe attributes that must be present
            bool skip_to_opening_tag;       ///<True if we are looking for the opening or the closing tag
        };

    };

}
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/xml/sax_reader.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: sax_reader.h,v $
 *-----------------------------------------------------------------------*/

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "shawn_config.h"

#ifdef HAVE_EXPAT 

#include "sys/xml/sax_reader.h"

#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cerrno>

#ifndef XMLCALL
#define XMLCALL
#endif

using namespace std;

namespace shawn
{
	namespace xml
	{
#define DELETE(__name__) if((__name__)) { delete (__name__); (__name__) = NULL; }

		// ----------------------------------------------------------------------
        /// Expat C-style callback funcion
		void XMLCALL saxreader_start(void *userdata, const char *name, const char **atts)
		{
			SAXReader* ptr = (SAXReader*) userdata;
			ptr->handle_start_element(name, atts);
		}

		// ----------------------------------------------------------------------
        /// Expat C-style callback funcion
		void XMLCALL saxreader_end(void *userdata, const char *name)
		{
			SAXReader* ptr = (SAXReader*) userdata;
			ptr->handle_end_element(name);
		}


		// ----------------------------------------------------------------------
        /// 
        /**
          */
        SAXReader::
            SAXReader() 
            : document_uri_(""), 
            is_(NULL), 
            parser(NULL), 
            stop_flag_(false)
        {
        }

		// ----------------------------------------------------------------------
        SAXReader::
            ~SAXReader()
        {
            reset();        
        }


		// ----------------------------------------------------------------------
        void SAXReader::
            set_document_uri( const std::string& s) 
            throw() 
        {
            document_uri_ = s;
        }

		// ----------------------------------------------------------------------
        void SAXReader::
           interrupt() 
           throw()
        {
            stop_flag_ = true;
        }


		// ----------------------------------------------------------------------
        void SAXReader::
            reset() 
            throw() 
        {
            //Destroy the parser if existing
            if( parser ) {
                XML_ParserFree(parser);
                parser = NULL;
            }

            //Close the input stream
            if( is_ ) {
                is_->close();
                is_ = NULL;
            }

            stop_flag_ = false;
        }

		// ----------------------------------------------------------------------
        void SAXReader::
            open_file() 
            throw(std::runtime_error) 
        {
            assert( document_uri_.c_str() != NULL );

            is_ = new std::ifstream();
            is_->open(document_uri_.c_str(), std::ios::in);

            if( ! (*is_) ) {
				std::cerr << "SAXReader: Unable to open file ("<< document_uri_.c_str() <<
					") for reading: " << strerror(errno) << std::endl;
				exit(1);
			}

        }

		// ----------------------------------------------------------------------
        void SAXReader::
            parse() 
            throw(std::runtime_error) 
        {
            char buf[16384];
            int len;

            //Open the file
            open_file();

			//Create the SAX parser and register the C-Style handlers
			//which will call back on this object instace
			parser = XML_ParserCreate(NULL);
			XML_SetUserData(parser, (void*)this);
			XML_SetElementHandler(parser, saxreader_start, saxreader_end);

            //Read the file until the end of file marker is encountered 
            //and pass the data to the sax xml parser
            while( !is_->eof() && !stop_flag_ ) 
            {
                is_->read( buf, sizeof(buf) );
                len = is_->gcount();

                if (XML_Parse(parser, buf, len, is_->eof()) == XML_STATUS_ERROR)
				{
					std::cerr << XML_ErrorString(XML_GetErrorCode(parser)) <<
						"at line " << XML_GetCurrentLineNumber(parser) << std::endl;

                    reset();
                    throw std::runtime_error("Error in parsing XML input");
				}

            }

            //Done -> Close the file and free all associated memory
			parsing_done();
            reset();

        }

		// ----------------------------------------------------------------------
		void 
			SAXReader::
			parsing_done() 
			throw()
		{}

		// ----------------------------------------------------------------------
        void SAXReader::
            handle_start_element(const char*, const char**) 
            throw(std::runtime_error)
        {
        }

		// ----------------------------------------------------------------------
        void SAXReader::
            handle_end_element(const char*) 
            throw(std::runtime_error)
        {
        }

        // ----------------------------------------------------------------------
        const char* SAXReader::
            attribute(const char* name, const char **atts, char* default_val /* = NULL */)  const
        {
            for(; *atts; atts += 2)
			    if( !strcmp(name, atts[0]) )
                    return atts[1];

            return default_val;
        }

        // *********************************************************************
        // *********************************************************************
        // *********************************************************************

		// ----------------------------------------------------------------------
        void SAXSkipReader::
            reset() 
            throw() 
        {
            SAXReader::reset();
            clear_skip_target();
        }

		// ----------------------------------------------------------------------
        /// 
        void SAXSkipReader::
            handle_start_element(const char *name, const char **atts) 
            throw(std::runtime_error)
        {
            SAXReader::handle_start_element(name, atts);
            if( check_skip_target_reached(name, atts, true) )
                skip_target_reached(name, atts);
        }

		// ----------------------------------------------------------------------
        void SAXSkipReader::
            handle_end_element(const char *name) 
            throw(std::runtime_error)
        {
            SAXReader::handle_end_element(name);
            if( check_skip_target_reached(name, NULL, false) )
                skip_target_reached(name, NULL);
        }



        // *********************************************************************
        // *********************************************************************
        // *********************************************************************


	    // ----------------------------------------------------------------------
        SAXSimpleSkipReader::
            SAXSimpleSkipReader()
           : skip_to_tag_( "" )
        {
            clear_skip_target();
        }

	    // ----------------------------------------------------------------------
        void SAXSimpleSkipReader::
            set_skip_target(string name, bool opening_tag /* = true */) 
        {
            assert(name != "");
            clear_skip_target();
            skip_to_tag_ = name;
            skip_to_opening_tag = opening_tag;
        }

		// ----------------------------------------------------------------------
        void SAXSimpleSkipReader::
            clear_skip_target() 
        {
            skip_to_tag_ = "";
            skip_to_tag_atts_.clear();
            skip_to_opening_tag = true;
            return;
        }

		// ----------------------------------------------------------------------
        /// Returns true if we are in skipping mode
        /**
          *
          */
        bool SAXSimpleSkipReader::
            skipping() 
        {
            return skip_to_tag_ != "";
        }

		// ----------------------------------------------------------------------
        void SAXSimpleSkipReader::
            set_skip_target_add_attr(string name, string value) 
        {
            skip_to_tag_atts_[name] = value;
        }

		// ----------------------------------------------------------------------
        bool SAXSimpleSkipReader::
            check_skip_target_reached(const char* name, const char** atts, bool opening_tag) 
        {
            const char* tmp;

			//Check if the current tag name equals the needed skip target
			if( this->skip_to_tag_ != std::string(name) )
				return false;

			//Check if we need to skip at all or the tag is of the same type
            if( !skipping() || opening_tag != skip_to_opening_tag)
                return false;

            //Check all attributes if we have reached an opening tag
            if( opening_tag ) 
            {
                for( str_str_map::iterator it = skip_to_tag_atts_.begin(); it != skip_to_tag_atts_.end(); it++) 
                {
                    tmp = attribute( (*it).first.c_str(), atts );
                    
                    //Attribute not found -> No match
                    if(!tmp)
                        return false;

                    //Attribute has wrong value -> No match
                    if( (*it).second != tmp )
                        return false;

                }
            }

            //Skip target reached
            clear_skip_target();
            return true;
        }

    }
}
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/xml/sax_reader.cpp,v $
* Version $Revision: 1.14 $
* Date    $Date: 2006/04/06 11:02:47 $
*-----------------------------------------------------------------------
* $Log: sax_reader.cpp,v $
 *-----------------------------------------------------------------------*/

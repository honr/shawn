/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "shawn_config.h"

#ifdef HAVE_EXPAT 

#include "sys/xml/sax_interruptible_reader.h"

extern "C" {
#include <expat.h>
}

#include <assert.h>
#include <string>
#include <iostream>
#include <fstream>

#ifndef XMLCALL
#define XMLCALL
#endif

using namespace std;

namespace shawn
{
	namespace xml
	{
		void XMLCALL saxreader_start(void *userdata, const char *name, const char **atts);
		void XMLCALL saxreader_end(void *userdata, const char *name);

		// ----------------------------------------------------------------------
		SAXInterruptibleReader::
			SAXInterruptibleReader()
			: initialized_(false)
		{}

		// ----------------------------------------------------------------------
		SAXInterruptibleReader::
			~SAXInterruptibleReader()
		{}

		// ----------------------------------------------------------------------
		void SAXInterruptibleReader::
			parse() 
			throw(runtime_error) 
		{
			char buf[16384];
			int len;

			if(!initialized_)
			{
				//Open the file
				open_file();

				//Create the SAX parser and register the C-Style handlers
				//which will call back on this object instace
				parser = XML_ParserCreate(NULL);
				XML_SetUserData(parser, (void*)this);
				XML_SetElementHandler(parser, saxreader_start, saxreader_end);
				initialized_ = true;
			}
			stop_flag_ = false;
			while(!cache_.empty() && !stop_flag_)
			{
				struct CacheData* cd = cache_.front();
				
				if( cd->open_tag ) {
					
					start_element(cd->name, cd->atts);
				}
					
				else
					end_element(cd->name);
					
				
				cache_.pop_front();
				delete cd;
			}

			//Read the file until the end of file marker is encountered 
			//and pass the data to the sax xml parser
			while( !is_->eof() && !stop_flag_ ) 
			{
				is_->read( buf, sizeof(buf) );
				len = is_->gcount();

				if (XML_Parse(parser, buf, len, is_->eof()) == XML_STATUS_ERROR)
				{
					cerr << XML_ErrorString(XML_GetErrorCode(parser)) <<
						"at line " << XML_GetCurrentLineNumber(parser) << endl;

					reset();
					throw runtime_error("Error in parsing XML input");
				}

			}

			if( !stop_flag_ )
			{
				//Done -> Close the file and free all associated memory
				parsing_done();
				reset();
			}
		}

		// ----------------------------------------------------------------------
		void SAXInterruptibleReader::
			handle_start_element(const char* name, const char** atts) 
			throw(runtime_error)
		{
			string _name = string(name);
			AttList attlist = convert(atts);
			
			if( stop_flag_ )
			{
				CacheData* cd = new CacheData;
				cd->open_tag = true;
				cd->name = _name ;
				cd->atts = attlist;
				cache_.push_back(cd);
			} 
			else {
				start_element(_name , attlist);
			}
		}


		// ----------------------------------------------------------------------
		void SAXInterruptibleReader::
			handle_end_element(const char* name) 
			throw(runtime_error)
		{			
			string n = string(name);
			if( stop_flag_ )
			{
				CacheData* cd = new CacheData;
				cd->open_tag = false;
				cd->name = n;
				cache_.push_back(cd);
			} 
			else 
				end_element(n);
		}


		// ----------------------------------------------------------------------
		SAXInterruptibleReader::AttList
			SAXInterruptibleReader::
			convert(const char **atts) 
			const
		{
			AttList attlist;

			for(; *atts; atts += 2)
				attlist.insert( pair<string, string>(string(atts[0]), string(atts[1])));

			return attlist;
		}

	}
}
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/xml/sax_interruptible_reader.cpp,v $
* Version $Revision: 1.2 $
* Date    $Date: 2006/04/07 15:22:56 $
*-----------------------------------------------------------------------
* $Log: sax_interruptible_reader.cpp,v $
 *-----------------------------------------------------------------------*/

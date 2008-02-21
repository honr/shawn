/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "shawn_config.h"
#include "sys/xml/sax_interruptible_reader.h"

#include <assert.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
using namespace irr;
using namespace irr::io;

namespace shawn
{
	namespace xml
	{
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
			stop_flag_ = false;
			if(!initialized_)
				open_file();

			while(!cache_.empty() && !stop_flag_)
			{
				struct CacheData* cd = cache_.front();
				
				if( cd->open_tag )
					start_element(cd->name, cd->atts);
				else
					end_element(cd->name);
				
				cache_.pop_front();
				delete cd;
			}

			//Read the file until the end of file
            while( irr_ && irr_->read() && !stop_flag_ ) 
            {
            	
            	switch( irr_->getNodeType()) 
            	{
            	//No xml node. This is usually the node if you did not read anything yet
            	case EXN_NONE: 
            		break;
            		
                //A xml element, like <foo>. 
            	case EXN_ELEMENT:
            		{
            		AttList atts;            		
            		for(int i = 0 ; i < irr_->getAttributeCount(); ++i)
            			atts.insert( pair<string, string>(string(irr_->getAttributeName(i)), string(irr_->getAttributeValue(i))));

                    handle_start_element(irr_->getNodeName(), atts);
            		}
            		break;
            		
                //End of an xml element, like </foo>. 
            	case EXN_ELEMENT_END:
                    handle_end_element(irr_->getNodeName());
            		break;
            		
                //Text within a xml element: <foo> this is the text. </foo>. 
            	case EXN_TEXT:
            		break;
            		
                //An xml comment like <!-- I am a comment --> or a DTD definition. 
            	case EXN_COMMENT:
            		break;
            		
                //An xml cdata section like <![CDATA[ this is some CDATA ]]>. 
            	case EXN_CDATA:
            		break;
            		
                //Unknown element. 
            	case EXN_UNKNOWN:
            		break;
            	}

            	//TODO: Validate the XML (e.g., a stack with open tags)
            }			

			//Done -> Close the file and free all associated memory
			if( !stop_flag_ )
			{
				parsing_done();
				reset();
			}
		}

		// ----------------------------------------------------------------------
		void SAXInterruptibleReader::
			handle_start_element(string name, AttList atts) 
			throw(runtime_error)
		{
			string _name = string(name);
			
			if( stop_flag_ )
			{
				CacheData* cd = new CacheData;
				cd->open_tag = true;
				cd->name = _name ;
				cd->atts = atts;
				cache_.push_back(cd);
			} 
			else 
			{
				start_element(_name, atts);
			}
		}


		// ----------------------------------------------------------------------
		void SAXInterruptibleReader::
			handle_end_element(string name) 
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

	}
}


/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/xml/sax_interruptible_reader.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: sax_interruptible_reader.cpp,v $
 *-----------------------------------------------------------------------*/

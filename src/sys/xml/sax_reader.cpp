/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include <sys/xml/sax_reader.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cerrno>
#include <sstream>

using namespace std;
using namespace irr;
using namespace irr::io;

namespace shawn
{
	namespace xml
	{

	    // ----------------------------------------------------------------------
	    std::string 
	        attribute(string name, AttList atts, std::string default_val /* = "" */)
	    {
	    	for(AttList::iterator it = atts.begin(), end = atts.end(); it!=end; ++it)
	    		if( it->first == name )
	    			return it->second;
	
	        return default_val;
	    }
	
	
		// ----------------------------------------------------------------------
        /// 
        /**
          */
        SAXReader::
            SAXReader() 
            : document_uri_(""), 
              irr_(NULL), 
              stop_flag_(false)
        {}

		// ----------------------------------------------------------------------
        SAXReader::
            ~SAXReader()
        {
            reset();        
        }

		// ----------------------------------------------------------------------
        void 
        	SAXReader::
            set_document_uri( const std::string& s) 
            throw() 
        {
            document_uri_ = s;
        }

		// ----------------------------------------------------------------------
        void 
        	SAXReader::
           interrupt() 
           throw()
        {
            stop_flag_ = true;
        }

		// ----------------------------------------------------------------------
        void 
        	SAXReader::
            reset() 
            throw() 
        {
            //Destroy the parser if existing
            if( irr_ ) {
                delete irr_;
                irr_ = NULL;
            }

            stop_flag_ = false;
        }

		// ----------------------------------------------------------------------
        void 
        	SAXReader::
            open_file() 
            throw(std::runtime_error) 
        {
        	//Create a new parser
        	assert( document_uri_.c_str() != NULL );
        	ifstream f(document_uri_.c_str());
        	irr_ = createIrrXMLReader( document_uri_.c_str() );
        	if(!f || !irr_)
        	{
				std::cerr << "SAXReader: Unable to read file ("<< document_uri_.c_str() << ")"<< std::endl;
				throw std::runtime_error("SAXReader: Unable to read file");
        	}
        }

		// ----------------------------------------------------------------------
        void 
        	SAXReader::
            parse() 
            throw(std::runtime_error) 
        {
            //Open the file
            open_file();

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
	            		

	            		element_stack_.push_front(irr_->getNodeName());
	                    handle_start_element(irr_->getNodeName(), atts);
	                    if( !irr_->isEmptyElement() )
	                    	break; //Do not break on an empty element, e.g. <foo/> 
            		}
            		
                //End of an xml element, like </foo>. 
            	case EXN_ELEMENT_END:
                    handle_end_element(irr_->getNodeName());

                    if(element_stack_.front() != irr_->getNodeName() ) 
                    {
                    	ostringstream os;
                    	os << 	"Invalid XML, expected closing tag for '" << element_stack_.front() << 
                    			"', but got '" << irr_->getNodeName() << "'" << endl;
                    	os << "Current XML stack: " << endl;
                    	
                    	for(deque<string>::reverse_iterator it = element_stack_.rbegin(), end = element_stack_.rend(); it!=end; ++it)
                    		os << "\t" << *it << endl;
                    	
                    	throw std::runtime_error(os.str());
                    }
            		element_stack_.pop_front();
            		if (element_stack_.size() == 0)
            			interrupt();
            		
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
        void 
        	SAXReader::
        	handle_start_element(std::string name, AttList atts) 
            throw(std::runtime_error)
        {}

		// ----------------------------------------------------------------------
        void 
        	SAXReader::
        	handle_end_element(std::string name)
        	throw(std::runtime_error)
        {}

        // *********************************************************************
        // *********************************************************************
        // *********************************************************************

		// ----------------------------------------------------------------------
        void 
        	SAXSkipReader::
            reset() 
            throw() 
        {
            SAXReader::reset();
            clear_skip_target();
        }

		// ----------------------------------------------------------------------
        /// 
        void 
        	SAXSkipReader::
        	handle_start_element(std::string name, AttList atts) 
            throw(std::runtime_error)
        {
            SAXReader::handle_start_element(name, atts);
            if( check_skip_target_reached(name, atts, true) )
                skip_target_reached(name, atts);
        }

		// ----------------------------------------------------------------------
        void SAXSkipReader::
        	handle_end_element(std::string name)
            throw(std::runtime_error)
        {
            SAXReader::handle_end_element(name);
            AttList atts;
            if( check_skip_target_reached(name, atts, false) )
                skip_target_reached(name, atts);
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
        void 
        	SAXSimpleSkipReader::
            set_skip_target(string name, bool opening_tag /* = true */) 
        {
            assert(name != "");
            clear_skip_target();
            skip_to_tag_ = name;
            skip_to_opening_tag = opening_tag;
        }

		// ----------------------------------------------------------------------
        void 
        	SAXSimpleSkipReader::
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
        bool 
        	SAXSimpleSkipReader::
            skipping() 
        {
            return skip_to_tag_ != "";
        }

		// ----------------------------------------------------------------------
        void 
        	SAXSimpleSkipReader::
            set_skip_target_add_attr(string name, string value) 
        {
            skip_to_tag_atts_[name] = value;
        }

		// ----------------------------------------------------------------------
        bool 
        	SAXSimpleSkipReader::
            check_skip_target_reached(string name, AttList atts, bool opening_tag) 
        {
            string tmp;

			//Check if the current tag name equals the needed skip target
			if( this->skip_to_tag_ != name )
				return false;

			//Check if we need to skip at all or the tag is of the same type
            if( !skipping() || opening_tag != skip_to_opening_tag)
                return false;

            //Check all attributes if we have reached an opening tag
            if( opening_tag ) 
            {
                for( str_str_map::iterator it = skip_to_tag_atts_.begin(); it != skip_to_tag_atts_.end(); it++) 
                {
                    tmp = attribute( (*it).first, atts );
                    
                    //Attribute not found -> No match
                    if(tmp == "")
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

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/xml/sax_reader.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: sax_reader.cpp,v $
 *-----------------------------------------------------------------------*/

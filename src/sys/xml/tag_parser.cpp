/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/xml/tag_parser.h"

#include <iostream>
#include <exception>

using namespace std;

namespace shawn
{
	namespace xml 
	{

		// ----------------------------------------------------------------------
		TagParser::
			~TagParser()
		{}

		// ----------------------------------------------------------------------
		void
			TagParser::
			handle_open_tag_tag(const char **atts, TagContainer& tc) 
			throw()
		{
			string name = to_string( attr("name", atts) );
			string type = to_string( attr("type", atts) );        
			string value = to_string( attr("value", atts) );
			TagFactoryHandle tfh = find_tag_factory(type);


			if( tfh.is_not_null() ) 
			{
				//Create a new tag
				TagHandle new_tag = tfh->create(type, name, value);
				if( new_tag.is_not_null() )
                {
					new_tag->set_persistency( true );
    				//Add it to the current tag container
    				if( has_current_container() )
	    				current_container(tc)->add_tag(new_tag);

    				//Make the created tag the current tag (for entries, subtags, ...)
    				current_tags_.push( new_tag.get() );
                }
				else
                {
					cerr << "LoadWorldFactory: Unable to create tag (name: " << 
						name<<", type: " << type << ", value: " << value << ")" << endl;
    				current_tags_.push( NULL );
                }
			} 
			else
			{
				cerr << "LoadWorldFactory: Unable to find factory for tag type: " << type << endl; 
				current_tags_.push(NULL);
			}
		}

		// ----------------------------------------------------------------------
		void
			TagParser::
			handle_close_tag_tag(const char **atts, TagContainer& tc) 
			throw() 
		{
			current_tags_.pop();
		}

		// ----------------------------------------------------------------------
		void 
			TagParser::
			handle_tag_entry(const char **atts)
			throw()
		{
			const char* index = attr("index", atts);
			const char* value = attr("value", atts);
			assert(value != NULL);

			//No factory for the current tag found -> Ignoring entries
			if(current_tags_.top() == NULL)
				return;

			//Index -> Map like, else Set like behavior
			if( index != NULL)
				current_tags_.top()->add_indexed_entry(index, value);
			else 
				current_tags_.top()->add_value_entry(value);
		}    

		// ----------------------------------------------------------------------
		bool 
			TagParser::
			has_current_container() 
			const throw()
		{
			if(current_tags_.empty() )
				return true;

			return current_tags_.top() != NULL;
		}

		// ----------------------------------------------------------------------
		shawn::TagContainer* 
			TagParser::
			current_container(shawn::TagContainer& tc) 
			const throw()
		{
			if( current_tags_.empty() )
				return &tc;

			return current_tags_.top();
		}

		// ----------------------------------------------------------------------
		TagFactoryHandle
			TagParser::
			find_tag_factory( const string& type )
			const throw( )
		{
			try 
			{
				assert(tfk_ != NULL);
				return tfk_->find_w(type);
			}
			catch( std::runtime_error ) 
			{
				cerr << "WARNING: ignoring tag(s) of unknown type '" << type << "'" << endl;
			}

			return NULL;
		}    

		// ----------------------------------------------------------------------
		std::string 
			TagParser::
			to_string(const char* cc)
			const throw()
		{
			return string( cc != NULL ? cc : "" );
		}

		// ----------------------------------------------------------------------
		const char* 
			TagParser::
			attr(const char* name, const char **atts)
			const throw()
		{
			for(; *atts; atts += 2)
				if( !strcmp(name, atts[0]) )
					return atts[1];

			return NULL;
		}

		// ----------------------------------------------------------------------
		void 
			TagParser::
			set_tag_factory_keeper(TagFactoryKeeper& tfk)
		{
			tfk_ = &tfk;
		}

	}
}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/xml/tag_parser.cpp,v $
* Version $Revision: 1.3 $
* Date    $Date: 2006/05/04 12:24:06 $
*-----------------------------------------------------------------------
* $Log: tag_parser.cpp,v $
 *-----------------------------------------------------------------------*/

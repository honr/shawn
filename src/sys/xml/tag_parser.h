/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_XML_TAG_PARSER_H
#define __SHAWN_SYS_XML_TAG_PARSER_H

#include "sys/tag.h"
#include "sys/taggings/tag_factory_keeper.h"

#include <stack>
#include <string>

namespace shawn
{
	namespace xml 
	{

		class TagParser
		{
		public:
			///
			virtual ~TagParser();

			///
			virtual void handle_open_tag_tag(const char **atts, shawn::TagContainer& tc) throw();

			///
			virtual void handle_close_tag_tag(const char **atts, shawn::TagContainer& tc) throw();      

			///
			virtual void handle_tag_entry(const char **atts) throw();

			///
			void set_tag_factory_keeper(TagFactoryKeeper&);

		private:
			typedef std::stack<shawn::Tag*> TagStack;

			///
			std::string to_string(const char*) const throw();

			///
			shawn::TagFactoryHandle find_tag_factory( const std::string& ) const throw();

			///
			bool has_current_container() const throw();

			///
			shawn::TagContainer* current_container(shawn::TagContainer&) const throw();

			///
			const char* attr(const char* name, const char **atts) const throw();
			
			TagStack current_tags_;			///< The current tags
			shawn::TagFactoryKeeper* tfk_;	///<
		};

	}
}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/xml/tag_parser.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: tag_parser.h,v $
 *-----------------------------------------------------------------------*/

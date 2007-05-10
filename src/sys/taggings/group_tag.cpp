/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/taggings/group_tag.h"

namespace shawn
{
   const std::string GroupTag::TAG_TYPE("group");

	GroupTag::
		GroupTag(const std::string& n, bool lock)
		: Tag(n, lock)
	{}
	// ----------------------------------------------------------------------
	GroupTag::
		~GroupTag()
	{}
	// ----------------------------------------------------------------------
	const std::string&
		GroupTag::
		type_identifier( void )
		const throw()
	{
		return TAG_TYPE;
	}
	// ----------------------------------------------------------------------
	std::string 
		GroupTag::
		encoded_content( void )
		const throw( std::runtime_error )
	{
		return "";
	}

}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/taggings/group_tag.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: group_tag.cpp,v $
 *-----------------------------------------------------------------------*/

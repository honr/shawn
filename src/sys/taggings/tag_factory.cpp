/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/taggings/tag_factory.h"

namespace shawn
{
    // ----------------------------------------------------------------------
	TagFactory::TagFactory()
		: world_(NULL)
	{}

    // ----------------------------------------------------------------------
	TagFactory::~TagFactory()
	{}

    // ----------------------------------------------------------------------
	void 
		TagFactory::
		set_world(World& w)
	{
		world_ = &w;
	}

    // ----------------------------------------------------------------------
	bool 
		TagFactory::
		has_world() 
		const
	{
		return world_ != NULL;
	}

    // ----------------------------------------------------------------------
	World& 
		TagFactory::
		world_w()
	{
		assert(world_ != NULL);
		return *world_;
	}

    // ----------------------------------------------------------------------
	const World& 
		TagFactory::
		world()
		const
	{
		assert(world_ != NULL);
		return *world_;
	}

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/taggings/tag_factory.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: tag_factory.cpp,v $
 *-----------------------------------------------------------------------*/

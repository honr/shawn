/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_PROTO_TRACINODEMOVEMENTCREATOR_H
#define __SHAWN_PROTO_TRACINODEMOVEMENTCREATOR_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TRACINODEMOVEMENTCREATOR
#include "sys/processor.h"
#include "TraCI_processor.h"

namespace traci {

	class TraCIControlledProcessor 
		: public shawn::Processor{
	public:
		virtual ~TraCIControlledProcessor(){}
		friend class traci::TraCIProcessor;

	protected:
		TraCIControlledProcessor(){}
	private:
		TraCIControlledProcessor(const TraCIControlledProcessor&);
	};

}
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/helloworld/helloworld_message.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: AutoCastApplication.h,v $
 *-----------------------------------------------------------------------*/

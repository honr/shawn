/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/comm_models/manual_communication_model.h"
#include "sys/comm_models/manual_communication_model_factory.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"

using namespace std;

namespace shawn
{

	// ----------------------------------------------------------------------
	ManualCommunicationModelFactory::
		~ManualCommunicationModelFactory()
	{}

	// ----------------------------------------------------------------------
	std::string 
		ManualCommunicationModelFactory::
		name(void) 
		const throw()
	{
		return "manual";
	}

	// ----------------------------------------------------------------------
	std::string 	
		ManualCommunicationModelFactory::
		description(void) 
		const throw()
	{
		return "Manual Communication Model";
	}

	// ----------------------------------------------------------------------
	CommunicationModel* 
		ManualCommunicationModelFactory::
		create( const SimulationController& sc) 
		const throw()
	{
		return new ManualCommunicationModel;
    }


}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/comm_models/manual_communication_model_factory.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: manual_communication_model_factory.cpp,v $
 *-----------------------------------------------------------------------*/

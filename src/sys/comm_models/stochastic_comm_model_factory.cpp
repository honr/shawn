/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/comm_models/stochastic_comm_model.h"
#include "sys/comm_models/stochastic_comm_model_factory.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"

#include <limits>

using namespace std;

namespace shawn
{

	// ----------------------------------------------------------------------
	StochasticCommunicationModelFactory::
		~StochasticCommunicationModelFactory()
	{}

	// ----------------------------------------------------------------------
	std::string 
		StochasticCommunicationModelFactory::
		name(void) 
		const throw()
	{
		return "stochastic";
	}

	// ----------------------------------------------------------------------
	std::string 	
		StochasticCommunicationModelFactory::
		description(void) 
		const throw()
	{
		return "Stochastic Communication Model";
	}

	// ----------------------------------------------------------------------
	CommunicationModel* 
		StochasticCommunicationModelFactory::
		create( const SimulationController& sc) 
		const throw()
	{
		double range = sc.environment().optional_double_param("range", std::numeric_limits<int>::max() );
		double sf = sc.environment().optional_double_param("smooth_factor", 0.25 );
		StochasticCommunicationModel* scm = new StochasticCommunicationModel( 0, sf );;

		if( range != std::numeric_limits<int>::max() )
			scm->set_transmission_range(range);

		return scm;
	}


}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/comm_models/stochastic_comm_model_factory.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: stochastic_comm_model_factory.cpp,v $
 *-----------------------------------------------------------------------*/

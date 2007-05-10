/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "shawn_config.h"
#ifdef HAVE_EXPAT 

#include "sys/simulation/environment_config_loader.h"
#include "sys/simulation/simulation_controller.h"

using namespace std;

namespace shawn
{

    // ----------------------------------------------------------------------
    EnvironmentConfigLoader::EnvironmentConfigLoader(SimulationController& sc) 
        : parsing_state_(Unknown), 
        sim_controller_(sc)
    {
		set_tag_factory_keeper( sc.tag_factory_keeper_w() );
	} 

    // ----------------------------------------------------------------------
    EnvironmentConfigLoader::~EnvironmentConfigLoader()  
    {} 

    // ----------------------------------------------------------------------
    void EnvironmentConfigLoader::skip_target_reached(const char*, const char** atts) 
    {}

    // ----------------------------------------------------------------------
    void EnvironmentConfigLoader::handle_start_element(const char *name, const char **atts) 
        throw(runtime_error)
    {
        SAXSkipReader::handle_start_element(name, atts);

        if( !skipping() )
            parse_xml(name, atts, true);
    }

    // ----------------------------------------------------------------------
    void EnvironmentConfigLoader::handle_end_element(const char *name) throw(runtime_error)
    {
        SAXSkipReader::handle_end_element(name);
        if( !skipping() )
            parse_xml(name, NULL, false);
    }

    // ----------------------------------------------------------------------
    inline void EnvironmentConfigLoader::parse_xml(const char *name, const char **atts, bool opening_tag) throw(runtime_error) 
    {
        if( parsing_state_ == Done )
            return;

        if(strcmp("scenario", name) == 0 && parsing_state_ == Unknown) 
        {
            opening_tag? parsing_state_ = Scenario : parsing_state_ = Done;
        }
        else if(strcmp("environment", name) == 0 && parsing_state_ == Scenario) 
        {
            opening_tag ? parsing_state_ = Environment : parsing_state_ = Done;
        }
        else if(strcmp("tag", name) == 0 && parsing_state_ == Environment) 
		{
			if(opening_tag)
				handle_open_tag_tag(atts, sim_controller_.environment_w());
			else
				handle_close_tag_tag(atts, sim_controller_.environment_w());
		}
        else if(strcmp("entry", name) == 0 && parsing_state_ == Environment && opening_tag) 
        {
            handle_tag_entry(atts);
        }

    }

}

#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/simulation/environment_config_loader.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: environment_config_loader.cpp,v $
 *-----------------------------------------------------------------------*/

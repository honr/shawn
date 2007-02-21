/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "shawn_config.h"
#include "sys/worlds/world_tasks.h"
#include "sys/worlds/processor_world_factory.h"
#include "sys/processors/processor_keeper.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/worlds/rect_world_factory.h"
#include "sys/worlds/cuboid_world_factory.h"
#include "sys/worlds/load_world_factory.h"
#include "sys/util/defutils.h"
#include "sys/misc/tokenize.h"

#include <iostream>

#ifdef HAVE_EXPAT
using namespace shawn::xml;
#endif
using namespace std;

namespace shawn
{
    void init_world_tasks( SimulationController& sc ) throw()
    {
        sc.simulation_task_keeper_w().add( new SimulationTaskRectWorldFactory );
        sc.simulation_task_keeper_w().add( new SimulationTaskCuboidWorldFactory );
#ifdef HAVE_EXPAT
        sc.simulation_task_keeper_w().add( new SimulationTaskLoadWorldFactory );
#endif
    }



    SimulationTaskProcessorWorldFactory::
        SimulationTaskProcessorWorldFactory()
    {}
    // ----------------------------------------------------------------------
    SimulationTaskProcessorWorldFactory::
        ~SimulationTaskProcessorWorldFactory()
    {}
    // ----------------------------------------------------------------------
    void
        SimulationTaskProcessorWorldFactory::
        run( SimulationController& sc )
        throw( std::runtime_error )
    {
        if( !sc.has_world() )
            throw std::runtime_error(std::string("World-filling task '") +
            name() +
            std::string("' needs a world to fill.") );

        World& w = sc.world_w();
        ProcessorWorldFactory* pwf = create_factory(sc);
        assert( pwf != NULL );

        try {
            set_node_count( sc, *pwf );
            add_processor_factories( sc, *pwf );
        }
        catch( std::runtime_error& ) {
            delete pwf;
            throw;
        }

        pwf->fill_world(w);

        delete pwf;
    }
    // ----------------------------------------------------------------------
    void
        SimulationTaskProcessorWorldFactory::
        set_node_count( SimulationController&  sc,
        ProcessorWorldFactory& pwf )
        throw( std::runtime_error )
    {
        pwf.set_node_count( sc.environment().required_int_param("count") );
    }
    // ----------------------------------------------------------------------
    void
        SimulationTaskProcessorWorldFactory::
        add_processor_factories( SimulationController&  sc,
        ProcessorWorldFactory& pwf )
        throw( std::runtime_error )
    {
        StrTok tok(sc.environment().optional_string_param("processors",""), ", ");
        for(StrTok::iterator it = tok.begin(), end = tok.end(); it!=end; ++it)
        {
            ProcessorFactoryHandle pfh = sc.processor_keeper_w().find_w( *it );
            assert( pfh != NULL ); // not found throws...
            pwf.add_processor_factory(pfh);
        }
    }










    SimulationTaskRectWorldFactory::
        SimulationTaskRectWorldFactory()
    {}
    // ----------------------------------------------------------------------
    SimulationTaskRectWorldFactory::
        ~SimulationTaskRectWorldFactory()
    {}
    // ----------------------------------------------------------------------
    std::string
        SimulationTaskRectWorldFactory::
        name( void )
        const throw()
    {
        return "rect_world";
    }
    // ----------------------------------------------------------------------
    std::string
        SimulationTaskRectWorldFactory::
        description( void )
        const throw()
    {
        return "fills a rectangle with random points";
    }
    // ----------------------------------------------------------------------
    ProcessorWorldFactory*
        SimulationTaskRectWorldFactory::
        create_factory( SimulationController& sc )
        throw( std::runtime_error )
    {
        RectWorldFactory* rwf = new RectWorldFactory;

        // todo: make this doubles...
        rwf->set_size( double(sc.environment().required_int_param("width")),
            double(sc.environment().required_int_param("height")) );

        return rwf;
    }










    SimulationTaskCuboidWorldFactory::
        SimulationTaskCuboidWorldFactory()
    {}
    // ----------------------------------------------------------------------
    SimulationTaskCuboidWorldFactory::
        ~SimulationTaskCuboidWorldFactory()
    {}
    // ----------------------------------------------------------------------
    std::string
        SimulationTaskCuboidWorldFactory::
        name( void )
        const throw()
    {
        return "cuboid_world";
    }
    // ----------------------------------------------------------------------
    std::string
        SimulationTaskCuboidWorldFactory::
        description( void )
        const throw()
    {
        return "fills a cuboid with random points";
    }
    // ----------------------------------------------------------------------
    ProcessorWorldFactory*
        SimulationTaskCuboidWorldFactory::
        create_factory( SimulationController& sc )
        throw( std::runtime_error )
    {
        CuboidWorldFactory* cwf = new CuboidWorldFactory;

        cwf->set_size(
            sc.environment().required_double_param("width"),
            sc.environment().required_double_param("width"),
            sc.environment().required_double_param("height") );

        return cwf;
    }










#ifdef HAVE_EXPAT

    SimulationTaskLoadWorldFactory::
        SimulationTaskLoadWorldFactory()
    {}
    // ----------------------------------------------------------------------
    SimulationTaskLoadWorldFactory::
        ~SimulationTaskLoadWorldFactory()
    {}
    // ----------------------------------------------------------------------
    std::string
        SimulationTaskLoadWorldFactory::
        name( void )
        const throw()
    {
        return "load_world";
    }
    // ----------------------------------------------------------------------
    std::string
        SimulationTaskLoadWorldFactory::
        description( void )
        const throw()
    {
        return "Reads topology from a XML file";
    }
    // ----------------------------------------------------------------------
    ProcessorWorldFactory*
        SimulationTaskLoadWorldFactory::
        create_factory( SimulationController& sc )
        throw( std::runtime_error )
    {
        LoadWorldFactory* xmlf = NULL;

        try {
            xmlf = new LoadWorldFactory;
            xmlf->set_document_uri( sc.environment().required_string_param("world_in_file") );
            xmlf->set_snapshot_id( sc.environment().optional_string_param("snapshot","0") );
        }
        catch( std::runtime_error& ) {
            delete xmlf;
            throw;
        }

        return xmlf;
    }
    // ----------------------------------------------------------------------
    void
        SimulationTaskLoadWorldFactory::
        set_node_count( SimulationController&,
        ProcessorWorldFactory& )
        throw( std::runtime_error )
    {
        // nothing -- xml factory needs no count
    }

#endif


}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/worlds/world_tasks.cpp,v $
* Version $Revision: 1.19 $
* Date    $Date: 2005/09/14 15:14:31 $
*-----------------------------------------------------------------------
* $Log: world_tasks.cpp,v $
 *-----------------------------------------------------------------------*/

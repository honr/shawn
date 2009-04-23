/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/


#ifndef __SHAWN_SYS_SIMULATION_SIMULATION_CONTROLLER_H
#define __SHAWN_SYS_SIMULATION_SIMULATION_CONTROLLER_H

#include "_apps_enable_cmake.h"

#include "shawn_config.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/logging/logger.h"
#include <string>
#include <cstdlib>

//#include "config/reading.h"
#ifdef ENABLE_READING

#ifdef WIN32

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#endif

#include "apps/reading/readings/reading.h"
#include "apps/reading/readings/reading_keeper.h"
#include "apps/reading/sensors/sensor.h"
#include "apps/reading/sensors/sensor_keeper.h"
//#include "apps/reading/sensors/sensor_factory.h"
#endif

namespace shawn
{
   class World;
   class SimulationTaskKeeper;
   class ProcessorKeeper;
   class RandomVariableKeeper;
   class EdgeModelKeeper;
   class TransmissionModelKeeper;
   class CommunicationModelKeeper;
   class DistanceEstimateKeeper;
   class HandleKeeperBase;
   class TagFactoryKeeper;

   /// Central repository for all simulation-relevant objects
   class SimulationController
#ifdef ENABLE_LOGGING
        : public Logger
#endif
   {
   public:
     typedef
        std::map<std::string,HandleKeeperBase*> NamedKeeperMap;


      ///@name construction / destruction
      ///@{
      ///
      SimulationController();
      ///
      virtual ~SimulationController();
      ///@}

      ///@name access to default keepers
      ///@{
      ///
      const SimulationEnvironment& environment( void ) const throw();
      ///
      SimulationEnvironment& environment_w( void ) throw();
      ///
      const SimulationTaskKeeper& simulation_task_keeper( void ) const throw();
      ///
      SimulationTaskKeeper& simulation_task_keeper_w( void ) throw();
      ///
      const ProcessorKeeper& processor_keeper( void ) const throw();
      ///
      ProcessorKeeper& processor_keeper_w( void ) throw();
      ///
      const RandomVariableKeeper& random_variable_keeper( void ) const throw();
      ///
      RandomVariableKeeper& random_variable_keeper_w( void ) throw();
      ///
      const EdgeModelKeeper& edge_model_keeper( void ) const throw();
      ///
      EdgeModelKeeper& edge_model_keeper_w( void ) throw();
      ///
      const TransmissionModelKeeper& transmission_model_keeper( void ) const throw();
      ///
      TransmissionModelKeeper& transmission_model_keeper_w( void ) throw();
      ///
      const CommunicationModelKeeper& communication_model_keeper( void ) const throw();
      ///
      CommunicationModelKeeper& communication_model_keeper_w( void ) throw();
      ///
      const DistanceEstimateKeeper& distance_estimate_keeper( void ) const throw();
      ///
      DistanceEstimateKeeper& distance_estimate_keeper_w( void ) throw();
      ///
      const TagFactoryKeeper& tag_factory_keeper( void ) const throw();
      ///
      TagFactoryKeeper& tag_factory_keeper_w( void ) throw();
      ///
#ifdef ENABLE_READING
      const reading::ReadingKeeper& reading_keeper( void ) const throw();
      ///
      reading::ReadingKeeper& reading_keeper_w( void ) throw();
      ///
      const reading::SensorKeeper& sensor_keeper( void ) const throw();
      ///
      reading::SensorKeeper& sensor_keeper_w( void ) throw();
      ///
#endif
      ///@}


      ///@name access to all keepers
      ///@{
      /// \returns a keeper with the given name and type. if no keeper
      /// of matching name exists, or the matching keeper is no T, NULL
      /// is returned.
      template<typename T>
      const T* keeper_by_name( const std::string& s )
         const throw()
      {
         NamedKeeperMap::const_iterator it = keeper_by_name_.find(s);
         if( it==keeper_by_name_.end() )
            return NULL;
         else
            return dynamic_cast<const T*>(it->second);
      }
      /// \returns a keeper with the given name and type. if no keeper
      /// of matching name exists, or the matching keeper is no T, NULL
      /// is returned.
      template<typename T>
      T* keeper_by_name_w( const std::string& s )
         throw()
      {
         NamedKeeperMap::iterator it = keeper_by_name_.find(s);
         if( it==keeper_by_name_.end() )
            return NULL;
         else
            return dynamic_cast<T*>(it->second);
      }
      ///@}

      ///@name access to the world
      ///@{
      ///
      bool has_world( void ) const throw();
      /// \returns the world, if has_world()
      World& world_w( void ) throw();
      /// \returns the world, if has_world()
      const World& world( void ) const throw();
      /// sets the world of the simulation. if there is already a world, it will
      /// be deleted.
      void set_world( World* ) throw();
      ///@}



      ///@name custom keepers
      ///@{
      /// adds a keeper. afterwards, it can be accessed via keeper_by_name()
      /// and keeper_by_name_w()
      void add_keeper( HandleKeeperBase* ) throw();
      ///@}

   protected:

   /// creates and initializes all default keepers
      void init_keepers( void ) throw();

   private:
      SimulationEnvironment* env_;
      World* world_;

      SimulationTaskKeeper* simulation_task_keeper_;
      ProcessorKeeper* processor_keeper_;
      RandomVariableKeeper* random_variable_keeper_;
      EdgeModelKeeper* edge_model_keeper_;
      TransmissionModelKeeper* transmission_model_keeper_;
      CommunicationModelKeeper* communication_model_keeper_;
      DistanceEstimateKeeper* distance_estimate_keeper_;
      TagFactoryKeeper* tag_factory_keeper_;
#ifdef ENABLE_READING
      reading::ReadingKeeper* reading_keeper_;
      reading::SensorKeeper* sensor_keeper_;
#endif
      NamedKeeperMap keeper_by_name_;
      inline SimulationController( const SimulationController& ) { abort(); }
   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/simulation/simulation_controller.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: simulation_controller.h,v $
 *-----------------------------------------------------------------------*/

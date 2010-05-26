/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISEML_SENSOR_UPDATER_H
#define __SHAWN_APPS_WISEML_SENSOR_UPDATER_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISEML
#include "apps/wiseml/sensors/wiseml_sensor.h"
#include "apps/wiseml/sensors/wiseml_raw_sensor.h"
#include "apps/reading/sensors/sensor_keeper.h"
#include "sys/simulation/simulation_controller.h"
 
using namespace shawn;
using namespace std;
namespace wiseml
{
   /** \brief Updates attached sensors
    */
   class WisemlSensorUpdater
   {
   public:
      WisemlSensorUpdater();
      virtual ~WisemlSensorUpdater();
      
      void register_updater(SimulationController &sc);

      void remove_sensor( string sensor_id );
      void update_sensor( string sensor_id, string value );

      WisemlRawSensor* operator[](string sensor_id);

      bool sensor_exists(string sensor_id);
	   map<string, WisemlRawSensor*> reg_sensors_;

   protected:
      typedef map<string, RefcntPointer<reading::SensorFactory> >::iterator SFIterator;
   };
}

#endif
#endif

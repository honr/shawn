/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISEML_SENSOR_H
#define __SHAWN_APPS_WISEML_SENSOR_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_READING
#ifdef ENABLE_WISEML
#include "sys/node.h"
#include "apps/reading/sensors/sensor.h"

using namespace shawn;
namespace wiseml
{
   /** \brief Base class for WiseML controlled Sensors.
    * A special sensor class for reading out capability values during
    * a wiseml controlled simulation.
    */
   class WiseMlSensor:
      public reading::Sensor
   {
   public:
      ///@name Contructor/Destructor
      ///@{
      WiseMlSensor(std::string, const Node&);
      virtual ~WiseMlSensor();
      ///@}
   };
}

#endif
#endif
#endif

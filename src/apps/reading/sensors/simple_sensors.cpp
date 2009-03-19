/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "apps/reading/sensors/simple_sensors.h"
namespace reading
{
   DoubleSensor::DoubleSensor(const DoubleReading &rdg,
                              const shawn::Node &node)
   : Sensor<double>(rdg,node)
   {
   }

   DoubleSensor::~DoubleSensor()
   {
   }

   double DoubleSensor::value()
   {
      return reading_.value(node_.real_position());
   }
}
#endif

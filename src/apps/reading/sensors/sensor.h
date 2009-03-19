/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_READING_SENSOR_H
#define __SHAWN_APPS_READING_SENSOR_H

#include "_apps_enable_cmake.h"
#ifdef	ENABLE_READING
#include "apps/reading/simple_reading.h"
#include "sys/node.h"

namespace reading
{
   template<typename T>
   class Sensor
   {
   public:
      typedef T ValueType;
      Sensor(const SimpleReading<ValueType> &rdg, const shawn::Node &node)
         :reading_(rdg),node_(node) {};
      virtual ~Sensor(){};

      virtual ValueType value() = 0;

   protected:

      const SimpleReading<ValueType> &reading_;
      const shawn::Node &node_;

   };
}
#endif
#endif

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_CONTROL_H_
#define __SHAWN_APPS_TESTBEDSERVICE_CONTROL_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "sys/simulation/simulation_controller.h"
#include "sys/util/keeper_managed.h"

namespace testbedservice
{

   class TestbedserviceControl
      : public shawn::KeeperManaged
   {
   public:
      TestbedserviceControl();
      virtual ~TestbedserviceControl();

      virtual std::string name( void ) const throw() = 0;
      virtual std::string description( void ) const throw() = 0;
   };

}

#endif
#endif

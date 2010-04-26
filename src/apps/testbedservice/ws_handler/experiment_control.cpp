/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/ws_handler/experiment_control.h"
#include "apps/testbedservice/core/shawn_serverH.h"
#include "sys/processors/processor_keeper.h"
#include "sys/worlds/processor_world_factory.h"
#include "sys/misc/random/basic_random.h"
#include "sys/logging/logger.h"
#include "sys/misc/tokenize.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"
#include "sys/processor.h"
#include <boost/bind.hpp>

namespace testbedservice
{
   static testbedservice::ExperimentControl *experiment_control_ = 0;
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   ExperimentControl::
   ExperimentControl()
      : controller_ (0),
         sc_( 0 )
   {}
   // ----------------------------------------------------------------------
   ExperimentControl::
   ~ExperimentControl()
   {}
   // ----------------------------------------------------------------------
   void
   ExperimentControl::
   init( shawn::SimulationController& sc, TestbedServiceClient& controller )
      throw()
   {
      sc_ = &sc;
      controller_ = &controller;
      experiment_control_ = this;
   }

}

namespace shawn_server
{

   static bool experiment_control_available( void )
   {
      if ( testbedservice::experiment_control_ )
      {
         return true;
      }
      else
         std::cerr << "Error processing ws call - experiment_control_ not set." << std::endl;

      return false;
   }

   // -----------------------------------------------------------------------
   // ------------- Web Service Calls Experiment Control --------------------
   // -----------------------------------------------------------------------
   static char version_[] = "2.1";
   SOAP_FMAC5 int SOAP_FMAC6 __shawnts__getVersion( soap*,
            shawnts__getVersion *shawnts__getVersion_,
            shawnts__getVersionResponse *shawnts__getVersionResponse_ )
   {
      shawnts__getVersionResponse_->return_ = version_;
      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __shawnts__setStartTime( soap*,
            shawnts__setStartTime *shawnts__setStartTime_,
            shawnts__setStartTimeResponse *shawnts__setStartTimeResponse_ )
   {
      // TODO !
      std::cout << "shawn::testbedservice::setStartTime" << std::endl;
      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __shawnts__flashPrograms( soap*,
            shawnts__flashPrograms *shawnts__flashPrograms_,
            shawnts__flashProgramsResponse *shawnts__flashProgramsResponse_ )
   {
      // TODO !
      std::cout << "shawn::testbedservice::flashPrograms" << std::endl;
      return SOAP_OK;
   }
}

#endif

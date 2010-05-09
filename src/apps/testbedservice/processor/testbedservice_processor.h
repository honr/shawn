/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_PROCESSOR_H
#define __SHAWN_APPS_TESTBEDSERVICE_PROCESSOR_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/core/testbedservice_client.h"
#include "apps/testbedservice/ws_handler/experiment_control.h"
#include "apps/testbedservice/util/types.h"
#include "sys/processor.h"
#include "sys/event_scheduler.h"


namespace testbedservice
{

   /**
    */
   class TestbedServiceProcessor
      : public shawn::Processor
   {
   public:

      typedef ExperimentControl::FlashProgram FlashProgram;

      ///@name Constructor/Destructor
      ///@{
      TestbedServiceProcessor();
      virtual ~TestbedServiceProcessor();
      ///@}

      ///@name Inherited from Processor
      ///@{
      virtual void boot() throw();
      ///@}

      ///@name Message Reception
      ///@{
      virtual void process_text_message( TextMessage& message ) throw();
      virtual void process_binary_message( BinaryMessage& message ) throw();
      ///@}

      ///@name Message Sending
      ///@{
      void send_text_message( std::string& message, MessageLevel level ) throw();
      void send_binary_message( int length, uint8_t *buffer ) throw();
      ///@}

      ///@name Promming/Configuration
      ///@{
      void flash_program( FlashProgram& program ) throw();
      ///@}

      // --------------------------------------------------------------------
      inline TestbedServiceClient& controller( void ) throw()
      { assert(controller_); return *controller_; }

   private:
      TestbedServiceClient *controller_;
   };


}

#endif
#endif

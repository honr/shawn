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

      enum MessageLevel
      {
         MESSAGE_LEVEL_TRACE = 0,
         MESSAGE_LEVEL_DEBUG,
         MESSAGE_LEVEL_INFO,
         MESSAGE_LEVEL_WARN,
         MESSAGE_LEVEL_ERROR,
         MESSAGE_LEVEL_FATAL
      };

      struct TextMessage
      {
         std::string source;
         time_t timestamp;
         std::string message;
         MessageLevel level;
      };

      struct BinaryMessage
      {
         std::string source;
         time_t timestamp;
         int size;
         uint8_t *buffer;
      };

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
      // --------------------------------------------------------------------
      inline TestbedServiceClient& controller( void ) throw()
      { assert(controller_); return *controller_; }

   private:
      TestbedServiceClient *controller_;
   };


}

#endif
#endif

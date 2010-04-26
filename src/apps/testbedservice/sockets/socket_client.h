/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_SOCKET_CLIENT_H_
#define __SHAWN_APPS_TESTBEDSERVICE_SOCKET_CLIENT_H_

#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "sys/simulation/simulation_controller.h"
#include <boost/thread.hpp>
#include <boost/threadpool.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/asio.hpp>
#include <list>
#include <queue>

namespace testbedservice
{

   class SocketClient
   {

      struct QueueData
      {
         int length;
         unsigned char buffer[0xff];
      };

   public:
      SocketClient();
      ~SocketClient();
      // --------------------------------------------------------------------
      bool start_client( const shawn::SimulationController& );
      bool send_data( unsigned char *buf, int length );

   private:

      boost::asio::io_service *io_service_;
      boost::asio::ip::tcp::socket *s_;

      std::string remotehost_;
      std::string remoteport_;
   };

}

#endif
#endif

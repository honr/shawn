/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/sockets/socket_client.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/node.h"
#include "sys/world.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace testbedservice
{

   SocketClient::
   SocketClient()
   {}
   // --------------------------------------------------------------------
   SocketClient::
   ~SocketClient()
   {
      delete io_service_;
      delete s_;
   }
   // --------------------------------------------------------------------
   bool
   SocketClient::
   start_client( const shawn::SimulationController& sc )
   {
      try
      {
         remotehost_ = sc.environment().required_string_param( "socket_remote_host" );
         remoteport_ = sc.environment().required_string_param( "socket_remote_port" );

         io_service_ = new boost::asio::io_service();
         tcp::resolver resolver(*io_service_);
         tcp::resolver::query query(tcp::v4(), remotehost_, remoteport_);
         tcp::resolver::iterator iterator = resolver.resolve(query);
         s_ = new tcp::socket(*io_service_);
         s_->connect(*iterator);
      }
      catch (std::exception& e)
      {
         std::cerr << "Exception in start_client: " << e.what() << "\n";
      }
   }
   // --------------------------------------------------------------------
   bool
   SocketClient::
   send_data( unsigned char *buffer, int length )
   {
      boost::asio::write( *s_, boost::asio::buffer( buffer, length ) );
   }

}

#endif

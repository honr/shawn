/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_WSNAPI_CLIENT_H_
#define __SHAWN_APPS_TESTBEDSERVICE_WSNAPI_CLIENT_H_

#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/util/types.h"
#include <boost/thread.hpp>
#include <boost/threadpool.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <string>
#include <list>
#include <queue>
#include <vector>

namespace wsnapi_client
{
   class WSNPortBindingProxy;
   class shawnts__send;
}

namespace testbedservice
{

   class WsnApiClient
   {

   public:
      typedef std::vector<std::string> NodeIdVector;
      typedef NodeIdVector::iterator NodeIdVectorIterator;
      // --------------------------------------------------------------------
      WsnApiClient();
      ~WsnApiClient();
      // --------------------------------------------------------------------
      void init( std::string uri );
      // --------------------------------------------------------------------
      void send_text_message( NodeIdVector dests, TextMessage& text, bool threaded = true ) throw();
      void send_binary_message( NodeIdVector dests, BinaryMessage& bin, bool threaded = true  ) throw();
      // --------------------------------------------------------------------
      wsnapi_client::WSNPortBindingProxy& wsnapi( void ) throw()
      { assert(wsnapi_); return *wsnapi_; }

   private:
      void call_send_message( wsnapi_client::shawnts__send *send );
      // --------------------------------------------------------------------
      std::string wsnapi_uri_;
      int max_pool_size_;
      wsnapi_client::WSNPortBindingProxy *wsnapi_;

      boost::mutex pool_mutex_, send_mutex_;
      boost::threadpool::pool pool_;
   };

}

#endif
#endif

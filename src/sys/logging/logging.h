/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_LOGGING_LOGGING_H
#define __SHAWN_SYS_LOGGING_LOGGING_H

#include "shawn_config.h"


#if defined ( HAVE_LOG4CXX )
   #include "log4cxx/logger.h"
#elif defined ( HAVE_LOG4CPLUS )
   #include "log4cplus/logger.h"
#endif


#define LOG_DEFAULT_NAME std::string("default")
#define LOG_USER_NAME std::string("user")


#ifndef DISABLE_LOGGING
   #define ENABLE_LOGGING
#endif

#ifdef NDEBUG
   #ifndef LOG_DEBUG_ON_RELEASE
      #define DISABLE_LOGLEVEL_DEBUG
   #endif
   #ifdef NLOG_INFO_ON_RELEASE
      #define DISABLE_LOGLEVEL_INFO
   #endif
#endif

#if defined ( HAVE_LOG4CXX )

   #define __LOGGER_OBJECT log4cxx::LoggerPtr
   #define __INIT_LOGGER_OBJECT( logname ) log4cxx::Logger::getLogger( logname )

   #define USER( message ) LOG4CXX_INFO( log4cxx::Logger::getLogger( LOG_USER_NAME ), message )

   #define DEBUG( logger, message ) LOG4CXX_DEBUG( logger, message )
   #define INFO( logger, message ) LOG4CXX_INFO( logger, message )
   #define WARN( logger, message ) LOG4CXX_WARN( logger, message )
   #define ERROR( logger, message ) LOG4CXX_ERROR( logger, message )
   #define FATAL( logger, message ) LOG4CXX_FATAL( logger, message )

#elif defined ( HAVE_LOG4CPLUS )

   #define __LOGGER_OBJECT log4cplus::Logger
   #define __INIT_LOGGER_OBJECT( logname ) log4cplus::Logger::getInstance( logname )

   #define USER( message ) LOG4CPLUS_INFO( log4cplus::Logger::getInstance( LOG_USER_NAME ), message )

   #define DEBUG( logger, message ) LOG4CPLUS_DEBUG( logger, message )
   #define INFO( logger, message ) LOG4CPLUS_INFO( logger, message )
   #define WARN( logger, message ) LOG4CPLUS_WARN( logger, message )
   #define ERROR( logger, message ) LOG4CPLUS_ERROR( logger, message )
   #define FATAL( logger, message ) LOG4CPLUS_FATAL( logger, message )

#elif defined ( DISABLE_LOGGING )

   #define __LOGGER_OBJECT
   #define __INIT_LOGGER_OBJECT( logname )

   #define USER( message ) std::cout << message << std::endl;

   #define DEBUG( logger, message )
   #define INFO( logger, message )
   #define WARN( logger, message )
   #define ERROR( logger, message )
   #define FATAL( logger, message )

#else

   #define __LOGGER_OBJECT std::string
   #define __INIT_LOGGER_OBJECT( logname ) logname

	#ifdef DEBUG 
	#undef DEBUG
	#endif
	#ifdef INFO 
	#undef INFO
	#endif
	#ifdef WARN 
	#undef WARN
	#endif
	#ifdef ERROR 
	#undef ERROR
	#endif
	#ifdef FATAL 
	#undef FATAL
	#endif

   #define USER( message ) std::cout << message << std::endl;

   #define DEBUG( logger, message ) std::cout << logger << ": " << message << std::endl;
   #define INFO( logger, message ) std::cout << logger << ": " << message << std::endl;
   #define WARN( logger, message ) std::cout << logger << ": " << message << std::endl;
   #define ERROR( logger, message ) std::cout << logger << ": " << message << std::endl;
   #define FATAL( logger, message ) std::cout << logger << ": " << message << std::endl;

#endif


#if defined ( DISABLE_LOGLEVEL_DEBUG )
   #undef DEBUG
   #define DEBUG( logger, message )
#endif


#if defined (DISABLE_LOGLEVEL_INFO)
   #undef INFO
   #define INFO( logger, message )
#endif


#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/logging/logging.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: logging.h,v $
 *-----------------------------------------------------------------------*/

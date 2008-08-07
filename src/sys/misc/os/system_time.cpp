/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "sys/misc/os/system_time.h"

namespace shawn
{

	// ----------------------------------------------------------------------
	SystemTime::
		SystemTime()
	{
		touch();
	}

	// ----------------------------------------------------------------------
	void
		SystemTime::
		touch()
		throw()
	{
		#ifdef SYS_MISC_OS_SYSTEM_TIME_UNIX
			gettimeofday(&last_touch_, NULL);
		#else
			last_touch_ = GetTickCount();
		#endif
	}

	#ifdef SYS_MISC_OS_SYSTEM_TIME_UNIX
	// ----------------------------------------------------------------------
	int
		SystemTime::
		diff(struct timeval* result, struct timeval* x, struct timeval* y)
	{
	   // Perform the carry for the later subtraction by updating y
	   if (x->tv_usec < y->tv_usec)
	   {
		 int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
		 y->tv_usec -= 1000000 * nsec;
		 y->tv_sec += nsec;
	   }

	   if (x->tv_usec - y->tv_usec > 1000000)
	   {
		   int nsec = (x->tv_usec - y->tv_usec) / 1000000;
	       y->tv_usec += 1000000 * nsec;
	       y->tv_sec -= nsec;
	   }

	   //Compute the time remaining to wait tv_usec is certainly positive
	   result->tv_sec = x->tv_sec - y->tv_sec;
	   result->tv_usec = x->tv_usec - y->tv_usec;

	   return x->tv_sec < y->tv_sec;
	}
	#endif

	// ----------------------------------------------------------------------
	unsigned long
		SystemTime::
		ms_since_last_touch()
	{
		#ifdef SYS_MISC_OS_SYSTEM_TIME_UNIX
			struct timeval	now, res;
			gettimeofday(&now, NULL);
			diff(&res, &now, &last_touch_ );
			return res.tv_sec * 1000 + (res.tv_usec / 1000);
		#else
			return GetTickCount() - last_touch_;
		#endif
	}

	// ----------------------------------------------------------------------
	void
		SystemTime::
		sleep(unsigned long ms)
		throw()
	{
		#ifdef SYS_MISC_OS_SYSTEM_TIME_UNIX
			usleep( 1000 * ms );
		#else
			Sleep( 1000 * ms );
		#endif
	}


}

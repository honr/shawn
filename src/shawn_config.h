/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_CONFIG_H
#define __SHAWN_CONFIG_H

//----Visual Studio and WIN32 specific-------
#ifdef WIN32

	//Disable warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
	#pragma warning( disable : 4290 )

	//Disable warning C4511: Copy constructor could not be generated
	#pragma warning( disable : 4511 )

	//Disable warning C4311: 'variable' : pointer truncation from 'type' to 'type'. A 64-bit pointer was truncated to a 32-bit int or 32-bit long.
	#pragma warning( disable : 4311 )

	//Disable warning C4996: "This function or variable may be unsafe. Consider using _controlfp_s instead"
	#pragma warning( disable : 4996 )

#endif
//-----------
//#define DISABLE_LOGGING
#define DISABLE_LOGLEVEL_DEBUG

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn_config_template.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: shawn_config_template.h,v $
 *-----------------------------------------------------------------------*/

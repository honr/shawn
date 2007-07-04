/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************
 **                                                                    **
 ** \author Axel Wegener <wegener@itm.uni-luebeck.de>                  **
 **                                                                    **
 ************************************************************************/
#ifndef __SHAWN_APPS_TCPIP_STORAGE_H
#define __SHAWN_APPS_TCPIP_STORAGE_H

#ifdef SHAWN
     #include <shawn_config.h>
     #include "../buildfiles/_apps_enable_cmake.h"
     #ifdef ENABLE_TCPIP
            #define BUILD_TCPIP
     #endif
#else
     #define BUILD_TCPIP
#endif


#ifdef BUILD_TCPIP

#include <list>
#include <string>

namespace tcpip
{

class Storage: private std::list<unsigned char>
{
private:
	unsigned int pos_;
	bool iterValid_;
	std::list<unsigned char>::const_iterator iter_;
	bool iterEndValid_;
	std::list<unsigned char>::const_iterator iterEnd_;

	// sortation of bytes forwards or backwards?
	bool bigEndian_;
	
//	void advance(int);

public:

	/// Standard Constructor
	Storage();

	/// Constructor, that fills the storage with an cahr array. If length is -1, the whole array is handed over
	Storage(unsigned char[], int length=-1);


	// Destructor
	virtual ~Storage();

	bool valid_pos();

	void reset();

	virtual unsigned char readChar() throw();
	virtual void writeChar(unsigned char) throw();

	virtual int readByte() throw();
	virtual void writeByte(int) throw();
	virtual void writeByte(unsigned char) throw();

	virtual std::string readString() throw();
	virtual void writeString(std::string s) throw();

	virtual int readShort() throw();
	virtual void writeShort(int) throw();

	virtual int readInt() throw();
	virtual void writeInt(int) throw();

	virtual float readFloat() throw();
	virtual void writeFloat( float ) throw();

        virtual double readDouble() throw();
        virtual void writeDouble( double ) throw();

	// Some enabled functions of the underlying std::list
	unsigned int size() const { return static_cast<unsigned int>(std::list<unsigned char>::size()); }

	std::list<unsigned char>::const_iterator begin() const { return std::list<unsigned char>::begin(); }
	std::list<unsigned char>::const_iterator end() const { return std::list<unsigned char>::end(); }

};

} // namespace tcpip

#endif // BUILD_TCPIP

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: $
 *-----------------------------------------------------------------------*/

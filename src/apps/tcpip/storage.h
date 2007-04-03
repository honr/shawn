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

#include "socket.h"

#ifdef BUILD_TCPIP

#include <list>
#include <string>

namespace tcpip
{

class Storage: public std::list<unsigned char>
{
private:
	unsigned int pos_;
	std::list<unsigned char>::iterator iter_;
	std::list<unsigned char>::iterator iterEnd_;
	

public:
	Storage();
	Storage(unsigned char[], int length=-1);
	virtual ~Storage();

	void advance(int);
	bool valid_pos();

	void reset();

	virtual int readChar() throw();
	virtual void writeChar(char) throw();

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
};

} // namespace tcpip

#endif // BUILD_TCPIP

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: $
 * Version $Revision: $
 * Date    $Date: $
 *-----------------------------------------------------------------------
 * $Log: $
 *-----------------------------------------------------------------------*/

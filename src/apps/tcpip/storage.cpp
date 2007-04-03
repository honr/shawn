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

#include "storage.h"

#ifdef BUILD_TCPIP

#include <assert.h>


using namespace std;

namespace tcpip
{

	// ----------------------------------------------------------------------
	Storage::Storage()
		: pos_(0), iter_(0), iterEnd_(0)
	{
	}

	// ----------------------------------------------------------------------
	Storage::Storage(unsigned char packet[], int length)
		: pos_(0), iter_(0), iterEnd_(0)
	{
		if (length == -1) length = sizeof(packet) / sizeof(char);
		
		for(int i = 0; i < length; ++i) push_back(packet[i]);
		iter_ = begin();
		iterEnd_ = end();
	}

	// ----------------------------------------------------------------------
	Storage::~Storage()
	{}

	// ----------------------------------------------------------------------
	void Storage::advance(int count)
	{
		assert ( count >= 0 );
	    
		if (count != 1) iterEnd_ = 0;
		pos_ += count;
		if (iter_ == 0) iter_ = begin();
		::advance(iter_, count);
	}

	// ----------------------------------------------------------------------
	bool Storage::valid_pos()
	{
	    if (iterEnd_ == end())
	    {
		return (iter_ != 0 && iter_ != end());
	    } else {
		bool v = (pos_ < size());
		if (v) iterEnd_ = end();
		return v;
	    }
	}

	// ----------------------------------------------------------------------
	void Storage::reset()
	{
		this->clear();
		pos_=0;
		iter_ = 0;
	}
	// ----------------------------------------------------------------------
	/**
	* Reads a char form the array
	* @return The read char (between 0 and 255)
	*/
	int Storage::readChar()	throw()
	{
		assert( valid_pos() );
		//iterator it = begin();
		//::advance(it, pos_);
		//int hb = (int) *it;
		if (iter_ == 0) iter_ = begin();
		int hb = (int) *iter_;
		advance(1);
		return hb < 0 ? hb + 256 : hb;
	}

	// ----------------------------------------------------------------------
	/**
	*
	*/
	void Storage::writeChar(char value) throw()
	{
		push_back(value);
		advance(1);
	}

	// ----------------------------------------------------------------------
	/**
	* Reads a byte form the array
	* @return The read byte (between 0 and 255)
	*/
	int Storage::readByte()	throw()
	{
		return readChar();
	}

	// ----------------------------------------------------------------------
	/**
	*
	*/
	void Storage::writeByte(int value) throw()
	{
		writeChar( (char) (value & 0xFF) );
	}

	// ----------------------------------------------------------------------
	/**
	*
	*/
	void Storage::writeByte(unsigned char value) throw()
	{
		writeChar( (char) (value) );
	}

	// -----------------------------------------------------------------------
	/**
	* Reads a string form the array
	* @return The read string
	*/
	std::string Storage::readString() throw()
	{
		string tmp;
		int len = readInt();
		for (int i = 0; i < len; i++)
			tmp += (char) readChar();
		return tmp;
	}

	// ----------------------------------------------------------------------
	/**
	* Writes a string into the array;
	* @param s		The string to be written
	*/
	void Storage::writeString(std::string s) throw()
	{
		writeInt(s.length());
		for (string::iterator it = s.begin(); it!=s.end() ; it++)
			writeChar(*it);
	}

	// ----------------------------------------------------------------------
	/**
	* Restores an integer, which was split up in two bytes according to the
	* specification, it must have been split by its row byte representation
	* with MSBF-order
	*
	* @return the unspoiled integer value (between -32767 and 32768)
	*/
	int Storage::readShort() throw()
	{
		int hb = readChar();
		int lb = readChar();

		int mult = ((int) (hb & 0x80)) > 0 ? -1 : 1;
		int num = ((hb << 8) & 0xFF00) | (lb & 0x00FF);

		if (mult < 0)
			num = ((int) ((~num) & 0xFFFF)) + 1;
		return mult * num;
	}

	// ----------------------------------------------------------------------
	void Storage::writeShort( int value ) throw()
	{
		writeChar( (char) ((value >> 8 )& 0xFF) );
		writeChar( (char) (value & 0xFF) );
	}

	// ----------------------------------------------------------------------

	/*
	* restores an integer, which was split up in four bytes acording to the
	* specification, it must have been split by its row byte representation
	* with MSBF-order
	*
	* @return the unspoiled integer value (between -2.147.483.647 and 2.147.483.648)
	*/
	int Storage::readInt() throw()
	{
		int hb = readChar();
		int lb1 = readChar();
		int lb2 = readChar();
		int lb3 = readChar();

		int mult = ((int) (hb & 0x80)) > 0 ? -1 : 1;
		int num = (hb << 24) | (lb1 << 16) | (lb2 << 8) | lb3;

		if (mult < 0)
			num = ((int) ((~num) & 0xFFFFFFFF)) + 1;
		return mult * num;
	}

	// ----------------------------------------------------------------------
	void Storage::writeInt( int value ) throw()
	{
		writeChar( (char) ((value >> 24 )& 0xFF) );
		writeChar( (char) ((value >> 16 )& 0xFF) );
		writeChar( (char) ((value >> 8 )& 0xFF) );
		writeChar( (char) (value & 0xFF) );
	}

	// ----------------------------------------------------------------------

	/*
	* restores a float , which was split up in four bytes acording to the
	* specification, it must have been split by its row byte representation
	* with MSBF-order
	*
	* @return the unspoiled float value
	*/
	float Storage::readFloat() throw()
	{
		int b1 = readChar();
		int b2 = readChar();
		int b3 = readChar();
		int b4 = readChar();

		int intvalue = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
		float value;
		memcpy(&value, &intvalue, 4);

		return value;
	}

	// ----------------------------------------------------------------------
	void Storage::writeFloat( float value ) throw()
	{
		int intvalue;
		memcpy(&intvalue, &value, 4);
		
		writeChar( (char) ( (intvalue >> 24)& 0xFF)  );
		writeChar( (char) ( (intvalue >> 16)& 0xFF)  );
		writeChar( (char) ( (intvalue >> 8) & 0xFF ) );
		writeChar( (char) ( (intvalue & 0xFF)) );
	}

}

#endif // BUILD_TCPIP

/*-----------------------------------------------------------------------
 * Source  $Source: $
 * Version $Revision: $
 * Date    $Date: $
 *-----------------------------------------------------------------------
 * $Log: $
 *-----------------------------------------------------------------------*/

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

//#define NULLITER static_cast<list<unsigned char>::iterator>(0)

namespace tcpip
{

	// ----------------------------------------------------------------------
	Storage::Storage()
	{
		init();
	}

	// ----------------------------------------------------------------------
	Storage::Storage(unsigned char packet[], int length)
	{
		// Length is calculated, if -1, or given
		if (length == -1) length = sizeof(packet) / sizeof(char);
		
		// Get the content
		for(int i = 0; i < length; ++i) push_back(packet[i]);

		init();
	}

	// ----------------------------------------------------------------------
	void Storage::init()
	{
		// Initialize local variables
		pos_=0;
		iterValid_ = iterEndValid_ = false;
		valid_pos();

                short a = 0x0102;
                unsigned char *p_a = reinterpret_cast<unsigned char*>(&a);
                bigEndian_ = (p_a[0] == 0x01); // big endian?
	}

	// ----------------------------------------------------------------------
	Storage::~Storage()
	{}

	// ----------------------------------------------------------------------
	bool Storage::valid_pos()
	{
		if (size() == 0) return false;

		// Check iterator iterEnd_ for validity
		if ( !iterEndValid_ )
		{
			iterEnd_ = end();
			iterEndValid_ = true;
		}

		// Check Iterator iter_ for validity
		if ( !iterValid_ ) 
		{
			iter_ = std::list<unsigned char>::begin();
			unsigned int i = 0;
			while ( i < pos_ 
				&& iter_ != iterEnd_)
			{
				++i;
				++iter_;
			}
			iterValid_ = true;
		}

		return (iter_ != iterEnd_);
	}

	// ----------------------------------------------------------------------
	unsigned int Storage::position() const
	{
		// pos_ contains everytime the correct position, 
		// even if iterValid == false
		return pos_;
	}

	// ----------------------------------------------------------------------
	void Storage::reset()
	{
		std::list<unsigned char>::clear();
		pos_=0;
		iterValid_ = false;
		iterEndValid_ = false;
	}

	// ----------------------------------------------------------------------
	/**
	* Reads a char form the array
	* @return The read char (between 0 and 255)
	*/
	unsigned char Storage::readChar()	throw()
	{
		assert( valid_pos() );

		unsigned char hb = *iter_;
		++iter_;
		++pos_;
		return hb;
	}

	// ----------------------------------------------------------------------
	/**
	*
	*/
	void Storage::writeChar(unsigned char value) throw()
	{
		push_back(value);
	}

	// ----------------------------------------------------------------------
	/**
	* Reads a byte form the array
	* @return The read byte (between 0 and 255)
	*/
	int Storage::readByte()	throw()
	{
		return static_cast<int>(readChar());
	}

	// ----------------------------------------------------------------------
	/**
	*
	*/
	void Storage::writeByte(int value) throw()
	{
		writeChar( static_cast<unsigned char>(value & 0xFF) );
	}

	// ----------------------------------------------------------------------
	/**
	*
	*/
	void Storage::writeByte(unsigned char value) throw()
	{
		writeChar( value );
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
		writeInt(static_cast<int>(s.length()));
		for (string::iterator it = s.begin(); it!=s.end() ; it++)
			writeChar(*it);
	}

	// ----------------------------------------------------------------------
	/**
	* Restores an integer, which was split up in two bytes according to the
	* specification, it must have been split by its row byte representation
	* with MSBF-order
	*
	* @return the unspoiled integer value (between -32768 and 32767)
	*/
	int Storage::readShort() throw()
	{
                short value = 0;
                unsigned char *p_value = reinterpret_cast<unsigned char*>(&value);
                if (bigEndian_)
                {
                        // network is big endian
                        p_value[0] = readChar();
                        p_value[1] = readChar();
                } else {
                        // network is big endian
                        p_value[1] = readChar();
                        p_value[0] = readChar();
                }
                return value;
        }

	// ----------------------------------------------------------------------
	void Storage::writeShort( int value ) throw()
	{
		assert(value >= -32768 && value <= 32767);

		short svalue = static_cast<short>(value);
		//assert(svalue == value);

                unsigned char *p_svalue = reinterpret_cast<unsigned char*>(&svalue);
                if (bigEndian_)
                {
                        // network is big endian
                        writeChar(p_svalue[0]);
                        writeChar(p_svalue[1]);
                } else {
                       // network is big endian
                       writeChar(p_svalue[1]);
                       writeChar(p_svalue[0]);
                }
	}

	// ----------------------------------------------------------------------

	/*
	* restores an integer, which was split up in four bytes acording to the
	* specification, it must have been split by its row byte representation
	* with MSBF-order
	*
	* @return the unspoiled integer value (between -2.147.483.648 and 2.147.483.647)
	*/
	int Storage::readInt() throw()
	{
                int value = 0;
                unsigned char *p_value = reinterpret_cast<unsigned char*>(&value);
                if (bigEndian_)
                {
                        // network is big endian
                        p_value[0] = readChar();
                        p_value[1] = readChar();
                        p_value[2] = readChar();
                        p_value[3] = readChar();
                } else {
                        // network is big endian
                        p_value[3] = readChar();
                        p_value[2] = readChar();
                        p_value[1] = readChar();
                        p_value[0] = readChar();
                }
                return value;
	}

	// ----------------------------------------------------------------------
	void Storage::writeInt( int value ) throw()
	{
                unsigned char *p_value = reinterpret_cast<unsigned char*>(&value);
                if (bigEndian_)
                {
                        // network is big endian
                        writeChar(p_value[0]);
                        writeChar(p_value[1]);
                        writeChar(p_value[2]);
                        writeChar(p_value[3]);
                } else {
                        // network is big endian
                        writeChar(p_value[3]);
                        writeChar(p_value[2]);
                        writeChar(p_value[1]);
                        writeChar(p_value[0]);
                }
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
                float value = 0;
                unsigned char *p_value = reinterpret_cast<unsigned char*>(&value);
                if (bigEndian_)
                {
                        // network is big endian
                        p_value[0] = readChar();
                        p_value[1] = readChar();
                        p_value[2] = readChar();
                        p_value[3] = readChar();
                } else {
                        // network is big endian
                        p_value[3] = readChar();
                        p_value[2] = readChar();
                        p_value[1] = readChar();
                        p_value[0] = readChar();
                }

        return value;

	}

	// ----------------------------------------------------------------------
	void Storage::writeFloat( float value ) throw()
	{
                unsigned char *p_value = reinterpret_cast<unsigned char*>(&value);
                if (bigEndian_)
                {
                        // network is big endian
                        writeChar(p_value[0]);
                        writeChar(p_value[1]);
                        writeChar(p_value[2]);
                        writeChar(p_value[3]);
                } else {
                        // network is big endian
                        writeChar(p_value[3]);
                        writeChar(p_value[2]);
                        writeChar(p_value[1]);
                        writeChar(p_value[0]);
                }
        }
        // ----------------------------------------------------------------------
        void Storage::writeDouble( double value ) throw ()
	{
                unsigned char *p_value = reinterpret_cast<unsigned char*>(&value);
                if (bigEndian_)
                {
                        // network is big endian
                        for (int i=0; i<8; ++i)
			{
                                writeChar(p_value[i]);
                        }
                } else {
                        // network is big endian
                        for (int i=7; i>=0; --i)
	                {
                                writeChar(p_value[i]);
                        }
                }
        }

        // ----------------------------------------------------------------------
        double Storage::readDouble( ) throw ()
	{
                double value = 0;
                unsigned char *p_value = reinterpret_cast<unsigned char*>(&value);
                if (bigEndian_)
                {
                        // network is big endian
                        for (int i=0; i<8; ++i)
		        {
                                p_value[i] = readChar();
                        }
                } else {
                        // network is big endian
                        for (int i=7; i>=0; --i) {
                                p_value[i] = readChar();
                        }
                }
                return value;
        }
}

#endif // BUILD_TCPIP

/*-----------------------------------------------------------------------
 * Source  $Source: $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: $
 *-----------------------------------------------------------------------*/

/*
 * This file provides test cases for the tcpip package, that can be executed by MiniCppUnit
 *
 * Author: Axel Wegener
 */

#include "MiniCppUnit.hxx"
#include "storage.h"
//#include "socket.h"
#include <math.h>

class tcpipUnitTests : public TestFixture<tcpipUnitTests>
{
public:
	TEST_FIXTURE( tcpipUnitTests )
	{
		TEST_CASE( testStorageChar );
		TEST_CASE( testStorageByte );
		TEST_CASE( testStorageString );
		TEST_CASE( testStorageShort );
		TEST_CASE( testStorageInt );
		TEST_CASE( testStorageFloat );
		TEST_CASE( testStorageDouble );
		TEST_CASE( testStorageMixed );
	}
	
	void testStorageChar()
	{
		tcpip::Storage s1;

		s1.writeChar('\0');
		s1.writeChar('Ü');
		s1.writeChar('\n');

		ASSERT_EQUALS( true, s1.valid_pos() );
		ASSERT( s1.readChar() == '\0' ); 
		ASSERT( (char) s1.readChar() == 'Ü' ); 
		ASSERT( s1.readChar() == '\n' ); 
		ASSERT_EQUALS( false, s1.valid_pos() );
	}

	void testStorageByte()
	{
		tcpip::Storage s1;
       
		s1.writeByte(0);
		s1.writeByte(255);
		s1.writeByte('}');

		ASSERT_EQUALS( true, s1.valid_pos() );
		ASSERT( s1.readByte() == 0 ); 
		ASSERT( s1.readByte() == 255 ); 
		ASSERT( s1.readByte() == 125 );
		ASSERT_EQUALS( false, s1.valid_pos() );
	}

	void testStorageString()
	{
		tcpip::Storage s1;
       
		s1.writeString("Dies ist ein TestString");
		s1.writeString("");
		s1.writeString("2. Text mit Sonderzeichen ü ö ß \n\0");

		ASSERT_EQUALS( true, s1.valid_pos() );
		ASSERT_EQUALS( "Dies ist ein TestString", s1.readString() ); 
		ASSERT_EQUALS( "", s1.readString() ); 
		ASSERT_EQUALS( "2. Text mit Sonderzeichen ü ö ß \n\0", s1.readString() ); 
		ASSERT_EQUALS( false, s1.valid_pos() );
	}

	void testStorageShort()
	{
		tcpip::Storage s1;
       
		s1.writeShort(0);
		s1.writeShort(32767);
		s1.writeShort(-32768);

		ASSERT_EQUALS( true, s1.valid_pos() );
		ASSERT_EQUALS( 0, s1.readShort() ); 
		ASSERT_EQUALS( 32767, s1.readShort() );
		ASSERT_EQUALS( -32768, s1.readShort() ); 
		ASSERT_EQUALS( false, s1.valid_pos() );
	}

	void testStorageInt()
	{
		tcpip::Storage s1;
       
		s1.writeInt(0);
		s1.writeInt(-32768);
		s1.writeInt(32767);
		s1.writeInt(INT_MIN);
		s1.writeInt(INT_MAX);

		ASSERT_EQUALS( true, s1.valid_pos() );
		ASSERT_EQUALS( 0, s1.readInt() ); 
		ASSERT_EQUALS( -32768, s1.readInt() ); 
		ASSERT_EQUALS( 32767, s1.readInt() );
		ASSERT_EQUALS( INT_MIN, s1.readInt() );
		ASSERT_EQUALS( INT_MAX, s1.readInt() );
		ASSERT_EQUALS( false, s1.valid_pos() );
	}

	void testStorageFloat()
	{
		tcpip::Storage s1;
       
		float f1 = (float) 0.000002;
		float f2 = (float) 0.000001;

		s1.writeFloat(0.0);
		s1.writeFloat(f1);
		s1.writeFloat(f2);

		ASSERT_EQUALS( true, s1.valid_pos() );
		ASSERT_EQUALS( (float) 0.0, s1.readFloat() ); 
		ASSERT_EQUALS( f1, s1.readFloat() ); 
		ASSERT_EQUALS( f2, s1.readFloat() );
		ASSERT_EQUALS( false, s1.valid_pos() );
	}

	void testStorageDouble()
	{
		tcpip::Storage s1;
       
		double d1 = 0.000002;
		double d2 = 0.000001;

		s1.writeDouble(0.0);
		s1.writeDouble(d1);
		s1.writeDouble(d2);

		ASSERT_EQUALS( true, s1.valid_pos() );
		ASSERT_EQUALS( 0.0, s1.readDouble() ); 
		ASSERT_EQUALS( d1, s1.readDouble() ); 
		ASSERT_EQUALS( d2, s1.readDouble() );
		ASSERT_EQUALS( false, s1.valid_pos() );
	}	

	void testStorageMixed()
	{
		tcpip::Storage s1;
       
		double d1 = 0.000002;
		double d2 = 0.000001;

		s1.writeFloat(0.0);
		s1.writeDouble(d1);
		s1.writeString("\0");
		s1.writeShort(42);

		ASSERT_EQUALS( true, s1.valid_pos() );
		ASSERT_EQUALS( (float) 0.0, s1.readFloat() ); 
		ASSERT_EQUALS( d1, s1.readDouble() ); 
		ASSERT_EQUALS( "\0", s1.readString() );
		ASSERT_EQUALS( 42, s1.readShort() ); 
		ASSERT_EQUALS( false, s1.valid_pos() );
	}

};

REGISTER_FIXTURE( tcpipUnitTests );

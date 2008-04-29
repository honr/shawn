/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_SIMPLE_READING_H
#define __SHAWN_APPS_SIMPLE_READING_H

#include "_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "apps/reading/reading.h"
#include "sys/misc/box.h"

namespace reading
{

    ///  Superclass for simple value readings
    /**  This class implements the superclass Reading
    * and is a template for different types of simple readings,
    * as there are integer, double, string or bool readings.
    */
    template<typename T>
    class SimpleReading
        : public Reading
    {
    public:
        typedef T ValueType;

        ///@name construction / destruction
        ///@{
        SimpleReading(){}
        ///
        virtual ~SimpleReading(){}
        ///@}


        ///@name data access
        ///@{
        /// \return the reading at position v
        virtual ValueType value( const shawn::Vec& v ) const throw() =0;
       /// \return the domain of the reading, i.e., the region in which
        /// value() returns meaningful values. It is feasible to query
        /// values outside domain().
        virtual shawn::Box domain( void ) const throw()
       { return shawn::Box::INFINITE_3D_SPACE; }
       ///@}
    };


    //---------------------------------------------------------------------
    ///  Integer Reading
    /**  This class is derived form the superclass Simplereading which is,
    * on his side, derived from the superclass Reading.
    * It contains Readings with integer values.
    */
    class IntegerReading
        : public SimpleReading<int>
    {
    public:
       	///@name construction / destruction
	///@{
       IntegerReading(){}
       ///
       virtual ~IntegerReading(){}
       ///@}
    };
   DECLARE_HANDLES(IntegerReading);

    //---------------------------------------------------------------------
    ///  Double Reading
    /**  This class is derived form the superclass Simplereading which is,
    * on his side, derived from the superclass Reading.
    * It contains Readings with double values.
    */
    class DoubleReading
        : public SimpleReading<double>
    {
    public:
        ///@name construction / destruction
	///@{
	DoubleReading(){}
	///
        virtual ~DoubleReading(){}
	///@}
    };
   DECLARE_HANDLES(DoubleReading);

    //---------------------------------------------------------------------
    ///  String Reading
    /**  This class is derived form the superclass Simplereading which is,
    * on his side, derived from the superclass Reading.
    * It contains Readings with string values.
    */
    class StringReading
        : public SimpleReading<std::string>
    {
    public:
       	///@name construction / destruction
	///@{
       StringReading(){}
	///
       virtual ~StringReading(){}
       ///@}
    };
   DECLARE_HANDLES(StringReading);

    //---------------------------------------------------------------------
    ///  Bool Reading
    /**  This class is derived form the superclass Simplereading which is,
    * on his side, derived from the superclass Reading.
    * It contains Readings with bool values.
    */
    class BoolReading
        : public SimpleReading<bool>
    {
    public:
       	///@name construction / destruction
	///@{
       BoolReading(){}
       ///
       virtual ~BoolReading(){}
       ///@}
    };
   DECLARE_HANDLES(BoolReading);

}

#endif
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/simple_reading.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: simple_reading.h,v $
 *-----------------------------------------------------------------------*/


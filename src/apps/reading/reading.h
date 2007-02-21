/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_READING_H
#define __SHAWN_APPS_READING_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#ifndef TRACE
#define TRACE(x) //std::cout<<x<<std::endl;	//show comments
#endif

#include "sys/util/keeper_managed.h"
#include "sys/world.h"
#include "sys/misc/box.h"
#include "apps/reading/xml_readings_specifications.h"

#include <vector>
#include <set>

namespace reading
{
    class ReadingKeeper;
    class ReadingChangedHandler;

    DECLARE_HANDLES(Reading);

    ///  Superclass for every type of reading.
    /**  Readings in general contain information about a world.
    * That can be for instance values like temperature values or sonic values.
    * Further on, in the derived classes, readings are classified as simple or
    * complex readings. Imagine a sensor on a node which measures the temperature.
    * The temperature values can be influenced by adjacent CPUs also placed
    * on this node. If you want to consider this, use the complex reading type.
    * Corresponding methods, enforcing constraints and dependencies on the complex
    * reading, have to be implemented there. If not, use the simple reading type.
    * The same applies to corresponding methods there.
    * Furthermore simple and complex readings are differentiated into basic
    * value types, as there are integer, double, string or just bool values.
    * All classes are implemented in the "simple_reading"-files and
    * "complex_reading"-files.
    *
    * Later on those readings, more exactly the information held by a reading,
    * can be used to feed algorithms or to process it in any imaginable way.
    *
    * The example reading "RandomHeatReading" is derived from the superclass
    * SimpleReading and implements alongside "constructor & destructor" the methods
    * "name", later on it is used to identify the reading, and "description" and
    * also a method called "value". To carry out the the methods
    * from the example reading, a task, itself inherited from "SimulationTask", is
    * implemented in the same module. A task itself has a name-, description- and a
    * run-method. The run-method has to call the value-method from the
    * RandomHeatReading. Later on, the task can be executed in the shawn console by
    * typing in the name of the task. For further details have a look at the module
    * "../shawn/apps/randomheat" and its files.
    *
    * To write an own Reading, at first you have to create a new module in the
    * appropriate directory, namely "../shawn/apps/MyModule". Then you have to
    * create a new class inherited from Reading. If you want to be more
    * specific, derive your own class from the SimpleReading- or the ComplexReading-
    * types. You will have to implement the value-method in your class, which gathers the
    * wanted information out of, for example a document, for further processing.
    * Afterwards, as already mentioned, you will need a task that runs your
    * reading. To get things work, have a look at "Writing your own module" and
    * "Writing your own task" in the "master.pdf/dvi".
    *
    *
    */

    class Reading
        : public shawn::KeeperManaged
    {
	public:
       	
		typedef std::pair<ReadingChangedHandler*, shawn::Box> RB;
		typedef std::vector<RB> ReadingChangedHandlerList;
		
		///@name construction / destruction
		///@{
		///
		Reading();
		///
		virtual ~Reading();
		///@}

		///@name world membership
		///@{
		/** Sets the world that contains this reading,
		 *  used by World
		 */
		virtual void set_world( shawn::World& )throw();
		/// \return the domain of the reading, i.e., the region in which
         	/// value() returns meaningful values. It is feasible to query
         	/// values outside domain().
	 	virtual shawn::Box domain( void ) const throw()
         	{ 
	 		return shawn::Box::INFINITE_3D_SPACE; 
		}
		///@}
		///
		/** called after world is setup and before first node is created.
		 *  used by World
		 */
		virtual void init( void ) throw();
		
		///@name class own methods
		///@{
		///	Adds a listener - ReadingChangedHandler - and a box used  
		/// 	by the listener to the vector ReadingChangedHandlerList. This is done to have the possibility to inform all
		///  	listeners, if a change of the values in the reading occurs 
		///	or on the other hand to inform the reading itself about changes regarding the listeners, 
		///	as there is for example a sensor movement, changing the box used by the sensor, because of a new position. Latter presumes a call of the method remove_changed_handler to foreclose the possibility that old values, as there are for example ReadingChangedHandler-Box-pairs are handled.     .	
		virtual void add_changed_handler(ReadingChangedHandler* rch, shawn::Box& b );
		///	Removes a listener - ReadingChangedHandler - and its Box
		///	out of the vector ReadingChangedHandlerList. For more Details read the comment regarding the add_changed_handler method.
		virtual void remove_changed_handler(ReadingChangedHandler* rch, shawn::Box& b );
		///@}
	protected:
        	///
		ReadingChangedHandlerList rbv;
		///
		shawn::World* world_;
		///
	///@name world membership
        ///@{
        /** returns true or false if this Reading already has a World or not */
        bool has_world( void ) const throw();
        /** \return const World that contains this Reading */
        virtual const shawn::World& world( void ) const throw();
        /** \return writable World that contains this Reading */
        virtual shawn::World& world_w( void ) throw();
        ///@}
    };
    
    
    //------------------------------------------------------------	
    class ReadingChangedHandler
    {
	public:
		///@name construction / destruction
		///@{
		///
		ReadingChangedHandler(){}
		///
        	virtual ~ReadingChangedHandler(){}
		///@}
		///@name class own method
		///@{
		/**	This method has to be called indirectly from the reading 
		 *	which has changed on timeout in order to inform every 
		 *	listener about its change. A listener has to inherit this 
		 *	class. For further details take a look at "randomheat_-
		 *	reading.cpp" -> "timeout()".
		 */ 
		virtual void reading_changed( Reading& r, shawn::Box& b) = 0;
		///
		/**	With this method all reading changed handlers are informed about adjacent settings.
		  *	The settings on their side are gained by the xml reading parser.
		  */ 
		virtual void receiving_settings(xmlreading::Settings&, std::string&) = 0;
		///
		/**	This method inserts a shawn::Vec & a reading value into a local map.
		  *	This map contains all values in the domain of the reading changed handler.  */
		virtual void insert_domain_value(const shawn::Vec&, std::string&) = 0;
		///
		/**	This method erases a shawn::Vec & a reading value out of a local map.
		  *	This map contains all values in the domain of the reading changed handler.  */		
		virtual void erase_domain_value(const shawn::Vec&) = 0;
		///@}
    };

}

#endif
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/reading.h,v $
 * Version $Revision: 1.7 $
 * Date    $Date: 2006/08/31 20:34:50 $
 *-----------------------------------------------------------------------
 * $Log: reading.h,v $
 *-----------------------------------------------------------------------*/


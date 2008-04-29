/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_SIMPLE_SENSOR_H
#define __SHAWN_APPS_SIMPLE_SENSOR_H

#include "_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "apps/reading/sensor.h"
#include "sys/node_movements/movement_observer.h"
#include "apps/reading/simple_reading.h"
#include "sys/misc/box.h"
#include "sys/vec.h"
#include "sys/node.h"

#include <vector>
#include <map>

namespace reading
{
	class SensorChangedHandler;
	
	///Superclass for simple sensors
	/** This class is a template for different types of simple sensors,
	*	as there are integer, double, string, bool sensors.
	*	A sensor has a certain granularity, which means a certain range in
	*	which the sensor delivers correct values. This value can be changed
	*	to run different types of simulations. Furthermore there are different methods
	*	which grant access to the sensor.
	*/
	template <class T>
	class SimpleSensor : public reading::Sensor, public reading::ReadingChangedHandler, public shawn::MovementObserver
	{
	public:
		///
		typedef T ValueType;
		///
		typedef std::vector<SensorChangedHandler*> SensorChangedHandlerList;
		///
		typedef std::map<shawn::Vec, std::string, xmlreading::VecComparison> DomainReadingValues;
		// ----------------------------------------------------------------------
		///@name construction / destruction
		///@{
		///	The Constructor receives a node and a reading. 
		///	Furthermore the granularity of the sensors range can be set here by just changing the Vec v information
		SimpleSensor(shawn::Node& n, SimpleReading<T>& sr, shawn::Vec v = shawn::Vec(10,10,10) ) : n_ (n), sr_ (sr), v_ (v) 
		{
			set_granularity(v);
			n_.observer_add(*this);
			const shawn::Vec& initial_position_vector = n.real_position();
			const shawn::Vec& dummy = shawn::Vec(0,0,0);
			observer_initial_zone(n, initial_position_vector, dummy);
		};
		///
		virtual ~SimpleSensor()
		{
			n_.observer_remove(*this);
		};
		///@}
		///
		///@name sensor access
		///@{
		///	This method is used to set the granularity variable, 
		///	which is later on used to define the measures of a box 
		void set_granularity(const shawn::Vec& v) throw()
		{
			granularity_ = v / 2;
		};
		/////
		///	\return the granularity of the sensor
		shawn::Vec get_granularity() throw()
		{
			return granularity_ * 2;
		};
		///
		///	\return the value of the reading at the nodes position
		virtual ValueType value() throw()
		{
			return sr_.value( n_.real_position() );
		};
		///
		///	\return the initial zone for this sensor
		shawn::Box& get_initial_zone() throw()
		{
			return initial_zone_;
		}
		///
		///	\return the current updated zone for this sensor
		shawn::Box& get_updated_zone() throw()
		{
			return updated_zone_;
		}
		///@}

		///-
		///@name movement observer methods
		///@{
		///	\return initial zone of the observer
		///	For information read movement_observer.h
		///	Furhtermore the initial zone is registered with the current 
		///	sensor at the ReadingChangedHandler-Box-Vector in order to 
		///	deliver correct values.
		shawn::Box observer_initial_zone ( shawn::Node& n, const shawn::Vec& newpos, const shawn::Vec& velo) throw()
		{
			shawn::Box b(newpos - granularity_, newpos + granularity_);
			initial_zone_  = b;
			updated_zone_ = b;
			sr_.add_changed_handler(this, b);
			return b;
		};
		///
		///	\return updated zone of the observer
		///	At first the old values are deleted out of the 
		///	ReadingChangedHandler-Box-vector, afterwards the updated zone 
		///	is registered with the current sensor at the ReadingChangedHandler-Box-Vector
		///	in order to deliver correct values.
		shawn::Box observer_update_zone ( shawn::Node& n, const shawn::Vec& newpos, const shawn::Vec& velo) throw()
		{
			shawn::Box b(newpos - granularity_, newpos + granularity_);
			//remove old values out of the "reading changed handler-box-vector rbv"
			sr_.remove_changed_handler(this, updated_zone_);
			//add new values to the "reading changed handler-box-vector rbv"
			sr_.add_changed_handler(this, b);
			//update private variable updated_zone_
			updated_zone_ = b;

			return b;
		};
		///@}
		///	
		///@name reading changed handler methods
		///@{	
		virtual void reading_changed(reading::Reading& r, shawn::Box& b) throw()
		{
			shawn::Vec v = n_.real_position();
			for(SensorChangedHandlerList::iterator it = schl_.begin(); it != schl_.end(); ++it)
			{
				//std::cout<<"r_c_h: notifying sensor changed handler"<<std::endl;
				(**it).sensor_changed(*this, v);
			}
		};
		///
		virtual void receiving_settings(xmlreading::Settings&, std::string&) throw(){}
		///
		virtual void insert_domain_value(const shawn::Vec&, std::string&) throw(){}
		///
		virtual void erase_domain_value(const shawn::Vec&) throw(){}
		///@}
		///
		///@name sensor changed handler related stuff
		///@{
		/**	adds a sensor changed handler (listener) to the current sensor */
		virtual void add_sensor_changed_handler(SensorChangedHandler* sch) throw()
		{
			bool handler_exists = false;
			for(SensorChangedHandlerList::iterator it = schl_.begin(); it != schl_.end(); ++it)
			{
				if( sch == *it)
				{
					handler_exists = true;
				}
			}
			if(!handler_exists)
			{
				schl_.push_back(sch);
			}
		};
		///
		/**	removes a sensor changed handler (listener) from the current sensor */
		virtual void remove_sensor_changed_handler(SensorChangedHandler* sch) throw()
		{
			for(SensorChangedHandlerList::iterator it = schl_.begin(); it != schl_.end(); ++it)
			{
				if(sch == *it)
				{
					schl_.erase(it);
					break; 
				}
			}
		};
		///@}	
	public:
		SensorChangedHandlerList schl_;
		///
		shawn::Node& n_;
		///
		reading::SimpleReading<T>& sr_;
		///
		shawn::Vec& v_;
	private:
		///
		shawn::Vec granularity_;
		///
		shawn::Box initial_zone_;
		///
		shawn::Box updated_zone_;
	};
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	///Integer sensor
	/** This class is used, when the world has an integer value environment, 
	* for example an integer reading   
	*/
	class IntegerSensor : public SimpleSensor<int>
	{
	public:
		///@name construction / destruction
		///@{
		IntegerSensor(shawn::Node& n, reading::SimpleReading<int>& sr, shawn::Vec v = shawn::Vec(10,10,10)) : 
		  SimpleSensor<int>(n, sr, v ){}
		  
		///
		  virtual ~IntegerSensor(){}
		  ///@}
		  ///@name keeper managed methods
		  ///@{
		  virtual std::string name() const throw() {return "integer_sensor";}
		  ///
		  virtual std::string description() const throw() {return "A sensor which responds to integer values";} 
		  ///@}
	};
	DECLARE_HANDLES(IntegerSensor);
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------    
	///String sensor
	/** This class is used, when the world has a string value environment, 
	* for example a string reading   
	*/
	class StringSensor : public SimpleSensor<std::string>
	{
	public:	
		///@name construction / destruction
		///@{
		StringSensor(shawn::Node& n, SimpleReading<std::string>& sr, shawn::Vec v = shawn::Vec(10,10,10)) : SimpleSensor<std::string>(n, sr, v){}
		///
		virtual ~StringSensor(){}
		///@}
		///@name keeper managed methods
		///@{
		virtual std::string name() const throw() {return "string_sensor";}
		///
		virtual std::string description() const throw() {return "A sensor which responds to string values";}
		///@}
	};
	DECLARE_HANDLES(StringSensor);
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	///Double sensor
	/** This class is used, when the world has a double value environment, 
	* for example a double reading   
	*/
	class DoubleSensor : public SimpleSensor<double>
	{
	public:	
		///@name construction / destruction
		///@{
		DoubleSensor(shawn::Node& n, SimpleReading<double>& sr, shawn::Vec v = shawn::Vec(10,10,10)) : SimpleSensor<double>(n, sr, v){}
		///
		virtual ~DoubleSensor(){}
		///@}
		///@name keeper managed methods
		///@{
		virtual std::string name() const throw() {return "double_sensor";}
		///
		virtual std::string description() const throw() {return "A sensor which responds to double values";}
		///@}
	}; 
	DECLARE_HANDLES(DoubleSensor);
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	///Bool sensor
	/** This class is used, when the world has a bool value environment, 
	* for example an bool reading   
	*/
	class BoolSensor : public SimpleSensor<bool>
	{
	public:	
		///@name construction / destruction
		///@{
		BoolSensor(shawn::Node& n, SimpleReading<bool>& sr, shawn::Vec v = shawn::Vec(10,10,10)) : SimpleSensor<bool>(n, sr, v){}
		///
		virtual ~BoolSensor(){}
		///@}
		///@name keeper managed methods
		///@{
		virtual std::string name() const throw() {return "bool_sensor";}
		///
		virtual std::string description() const throw() {return "A sensor which responds to bool values";}
		///@}
	};
	DECLARE_HANDLES(BoolSensor);
	
}

#endif
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/reading/simple_sensor.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------*/


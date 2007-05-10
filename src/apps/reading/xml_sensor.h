/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_XML_SENSOR_H
#define __SHAWN_APPS_XML_SENSOR_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "apps/reading/simple_sensor.h"
#include "apps/reading/simple_reading.h"
#include "apps/reading/xml_reading.h"
#include "apps/reading/xml_readings_specifications.h"
#include "sys/util/string_conv.h"
#include "sys/vec.h"
#include "sys/node.h"
#include "sys/misc/box.h"

#include <math.h>

namespace xmlreading
{
	template <class T>
	class XMLSensor : public reading::SimpleSensor<T>
	{
	public:
		///
		typedef T ValueType;
		///
		typedef std::map<shawn::Vec, std::string, xmlreading::VecComparison> SensorDomainValues;
		///
		typedef std::map<double, shawn::Vec> EuklidianScalarProduktMap;
		///
		///@name construction / destruction
		///@{
		XMLSensor(shawn::Node& n, reading::SimpleReading<T>& sr, shawn::Vec v = shawn::Vec(10,10,10)) : reading::SimpleSensor<T>(n, sr, v )
		{
			TRACE("XMLSENSOR: initial_zone: "<< reading::SimpleSensor<T>::get_initial_zone().lower() << " "<< reading::SimpleSensor<T>::get_initial_zone().upper());
			reading::SimpleSensor<T>::sr_.add_changed_handler(this, reading::SimpleSensor<T>::get_initial_zone());
			sensor_settings_ = new xmlreading::Settings;
			sensor_readings_ = new SensorDomainValues;
			euklid_ = new EuklidianScalarProduktMap;
		};
		///
		virtual  ~XMLSensor()
		{
			reading::SimpleSensor<T>::sr_.remove_changed_handler(this, reading::SimpleSensor<T>::get_updated_zone());
			sensor_settings_ = NULL;
			sensor_readings_ = NULL;
			euklid_ = NULL;
			delete sensor_settings_;
			delete sensor_readings_;
			delete euklid_;
		}
		///@}
		// ----------------------------------------------------------------------
		///@name data access
		///@{
		virtual const double get_interpolated_value() const throw()
		{
			return interpolated;
		}
		///@}
		// ----------------------------------------------------------------------
		///@name reading changed handler methods
		///@{
		/** Have a look at the the ReadingChangedHandler class for more information */ 
		// ----------------------------------------------------------------------		
		///
		virtual void reading_changed(reading::Reading& r, shawn::Box& b) throw()
		{
			TRACE("XMLSENSOR: reading changed");
			if(interpolation)
				interpolate();
			shawn::Vec v = reading::SimpleSensor<T>::n_.real_position();
			for(typename reading::SimpleSensor<T>::SensorChangedHandlerList::iterator it = reading::SimpleSensor<T>::schl_.begin(); it != reading::SimpleSensor<T>::schl_.end(); ++it)
			{
				(**it).sensor_changed(*this, v);
			}
		}
		///
		virtual void receiving_settings(xmlreading::Settings& settings, std::string& reading_data_type) throw()
		{
			TRACE("XMLSENSOR: receiving settings");
			sensor_settings_ = &settings;
			reading_data_type_ = reading_data_type;
			adopt_settings();
		}
		///
		virtual void insert_domain_value(const shawn::Vec& vector, std::string& value) throw()
		{
			TRACE("XMLSENSOR: insert domain value");
			for(SensorDomainValues::iterator it = sensor_readings_->begin(); it != sensor_readings_->end(); ++it)
			{
				if(it->first == vector)
				{
					sensor_readings_->erase(it);
					break;
				}
			}
			std::pair<shawn::Vec, std::string> domain_val (vector, value);
			sensor_readings_->insert(domain_val);
			TRACE_READINGS(show_sensor_readings());
			calculate_relative_position_vector(vector);
			
			//std::cout<<"sensor_node_id:      "<<n_.id()<<std::endl;
			//std::cout<<"sensor_domain:       "<<get_initial_zone()<<std::endl;;
			//std::cout<<"sensor_reading_size: "<<sensor_readings_->size();
		}
		///
		virtual void erase_domain_value(const shawn::Vec& vector) throw()
		{
			TRACE("XMLSENSOR: erase domain value");
			for(SensorDomainValues::iterator it = sensor_readings_->begin(); it!=sensor_readings_->end(); ++it)
			{
				if(it->first == vector)
				{
					sensor_readings_->erase(it);
					erase_euklidian_value(vector);
					break;
				}
			}
		}
		///@}
	private:
		///@name interpolation related methods
		///@{
		///	Main interpolation methods
		/**	Interpolate a certain number of values, defined by the XML reading file.
		  *	The values needed for the interpolation are gathered out of the SensorDomainValues map - position specific values are held there -
		  *	and the EuklidianScalarProduktMap map - position vectors standardized after the euklidian norm are held there - 
		  *
		  **/
		virtual double interpolate() throw()
		{
			TRACE("XMLSENSOR: interpolate");
			interpolated = 0;
			if(reading_data_type_ == "string" | reading_data_type_ == "bool")
				std::cerr<<"No interpolation possible with string or bool values!"<<std::endl;
			else
			{	
				int setting_value = 0;
				double denominator = 0;
				for(xmlreading::Settings::iterator it = sensor_settings_->begin(); it!=sensor_settings_->end(); ++it)
				{
					std::string setting_name_ = (*it)->name;
					if(setting_name_ == "interpolation")
					{
						setting_value = shawn::conv_string_to_int((*it)->value);
						break;
					}
				};
				int counter = setting_value;
				for(EuklidianScalarProduktMap::iterator enm = euklid_->begin(); enm != euklid_->end(); ++enm)
				{
					if(counter == 0)
						break; 
					else
					{
						denominator += enm->first;
						--counter;
					}
				}
				counter = setting_value;
				for(EuklidianScalarProduktMap::iterator enm = euklid_->begin(); enm != euklid_->end(); ++enm)
				{
					if(counter == 0)
						break; 
					else
					{
						if(euklid_->size() == 0)
							interpolated = 0;
						else if(euklid_->size() == 1)
							interpolated = shawn::conv_string_to_double((sensor_readings_->find(enm->second))->second);
						else
						{
							double result = 0;
							for(SensorDomainValues::iterator sdv = sensor_readings_->begin(); sdv != sensor_readings_->end(); ++sdv)
							{
								if(enm->second == sdv->first)
								{
									result = (1- ((denominator - enm->first)/denominator))*shawn::conv_string_to_double(sdv->second);
									break;
								}
							}
							interpolated += result;
							--counter;
						} 
					}
				}
			}
			return interpolated;
		}
		///	Relative position calulation method
		/**	This method calculates the position vectors regarding the real position of the node.
		  **/
		void calculate_relative_position_vector(const shawn::Vec& vector) throw()
		{
			TRACE("XMLSENSOR: calculate relative position vector");
			shawn::Vec node_position = reading::SimpleSensor<T>::n_.real_position();
			shawn::Vec* pos_vector = new shawn::Vec((node_position.x() - vector.x()),(node_position.y() - vector.y()),(node_position.z() - vector.z()));
			calculate_euklidian_scalar_product(pos_vector, vector);
	 	}
		///
		///	Euklidian scalar product calculation
		/**	This method calculates the euklidian scalar produkt regarding a passed position vector **/
		void calculate_euklidian_scalar_product(shawn::Vec* position_vector, const shawn::Vec& vector) throw()
		{
			TRACE("XMLSENSOR: calculate euklidian scalar product");
			double x = position_vector->x();
			double y = position_vector->y();
			double z = position_vector->z();
			double scalar = sqrt( x*x + y*y + z*z );
			std::pair<double, shawn::Vec> result (scalar, vector); 
			for(EuklidianScalarProduktMap::iterator it = euklid_->begin(); it != euklid_->end(); ++it)
			{
				if(it->second == vector)
				{
					euklid_->erase(it);
					break;
				}
			}
			euklid_->insert(result);
			TRACE_READINGS(show_euklidian_value_map());
		}
		///
		/** Erases an euklidian scalar product value out of the scalar product map */
		void erase_euklidian_value(const shawn::Vec& vector) throw()
		{
			TRACE("XMLSENSOR: erase euklidian scalar product");
			for(EuklidianScalarProduktMap::iterator enm = euklid_->begin(); enm != euklid_->end(); ++enm)
			{
				if(enm->second == vector)
				{
					euklid_->erase(enm);
					break;
				}	
			}
			TRACE_READINGS(show_euklidian_value_map());
		}
		///@}
		// ----------------------------------------------------------------------
		///@name class own methods
		///@{
		/**	New settings can be implemented here.
		  *	For now, just the "interpolation"-method has been implemented.
		  *	Be carefull when creating a xml reading file. Do not set the interpolation setting twice!
		  *	This will end up in wrong interpolated values.
		  **/
		virtual void adopt_settings() throw()
		{
			TRACE("XMLSENSOR: adopting settings");
			for(xmlreading::Settings::iterator it = sensor_settings_->begin(); it!=sensor_settings_->end(); ++it)
			{  
				std::string setting_name_ = (*it)->name;
				if(setting_name_ == "interpolation")
				{
					interpolation = true;
					interpolate();
					TRACE("XMLSENSOR: interpolated value is: "<<interpolated);
				}else 
				{
					std::cerr<<"No appropriate setting found"<<std::endl;
				}
			}
		}
		// ----------------------------------------------------------------------
		/** With this method you can see the current sensor readings map with its values */
		void show_sensor_readings() throw()
		{
			TRACE("XMLSENSOR: showing readings");
			for(SensorDomainValues::iterator it = sensor_readings_->begin(); it != sensor_readings_->end(); ++it)
			{
				std::cout<<"pos: "<<it->first.x()<<" "<<it->first.y()<<" "<<it->first.z()<<"  val: "<<it->second<<std::endl;
			}
		}
		///
		// ----------------------------------------------------------------------
		/** With this method you can see the current euklidian value map with its values. */
		void show_euklidian_value_map() throw()
		{
			TRACE("XMLSENSOR: showing euklidian value map");
			for(EuklidianScalarProduktMap::iterator it = euklid_->begin(); it!= euklid_->end(); ++it)
			{
				std::cout<<"euklid: "<<it->first<<" Vector: "<<it->second.x()<<" "<<it->second.y()<<" "<<it->second.z()<<std::endl;
			}
		}
		///@}
	private:
		///
		SensorDomainValues* sensor_readings_;
		///
		EuklidianScalarProduktMap* euklid_;
		///
		xmlreading::Settings* sensor_settings_;
		///
		std::string reading_data_type_;
		///
		bool interpolation;
		///
		double interpolated;
	};
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	///XML Integer sensor
	/** This class is used, when the world has an integer value environment, 
	* for example an integer reading   
	*/
	class XMLIntegerSensor : public XMLSensor<int>
	{
	public:
		///@name construction / destruction
		///@{
		XMLIntegerSensor(shawn::Node& n, reading::SimpleReading<int>& sr, shawn::Vec v = shawn::Vec(10,10,10)): 
		  XMLSensor<int>(n, sr, v ){}
		///  
		  virtual ~XMLIntegerSensor(){}
		///@}
		///@name keeper managed methods
		///@{
		virtual std::string name() const throw() {return "xml_integer_sensor";}
		///
		virtual std::string description() const throw() {return "A sensor which responds to integer values";}
		///@}
	}; 
	DECLARE_HANDLES(XMLIntegerSensor);
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	///XML Double sensor
	/** This class is used, when the world has a double value environment, 
	* for example an integer reading   
	*/
	class XMLDoubleSensor : public XMLSensor<double>
	{
	public:
		///@name construction / destruction
		///@{
		XMLDoubleSensor(shawn::Node& n, reading::SimpleReading<double>& sr, shawn::Vec v = shawn::Vec(10,10,10)): 
		  XMLSensor<double>(n, sr, v ){}
		///  
		  virtual ~XMLDoubleSensor(){}
		///@}
		///@name keeper managed methods
		///@{
		virtual std::string name() const throw() {return "xml_double_sensor";}
		///
		virtual std::string description() const throw() {return "A sensor which responds to double values";}
		///@}
	}; 
	DECLARE_HANDLES(XMLDoubleSensor);
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	///XML Bool sensor
	/** This class is used, when the world has a bool value environment, 
	* for example an integer reading   
	*/
	class XMLBoolSensor : public XMLSensor<bool>
	{
	public:
		///@name construction / destruction
		///@{
		XMLBoolSensor(shawn::Node& n, reading::SimpleReading<bool>& sr, shawn::Vec v = shawn::Vec(10,10,10)): 
		  XMLSensor<bool>(n, sr, v ){}
		///   
		  virtual ~XMLBoolSensor(){}
		///@}
		///@name keeper managed methods
		///@{  
		virtual std::string name() const throw() {return "xml_bool_sensor";}
		/// 
		virtual std::string description() const throw() {return "A sensor which responds to bool values";}
		///@}
	}; 
	DECLARE_HANDLES(XMLBoolSensor);
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	///XML String sensor
	/** This class is used, when the world has a string value environment, 
	* for example an integer reading   
	*/
	class XMLStringSensor : public XMLSensor<std::string>
	{
	public:
		
		///@name construction / destruction
		///@{
		XMLStringSensor(shawn::Node& n, reading::SimpleReading<std::string>& sr, shawn::Vec v = shawn::Vec(10,10,10)): 
		  XMLSensor<std::string>(n, sr, v ){}
		///  
		  virtual ~XMLStringSensor(){}
		///@}
		///@name keeper managed methods
		///@{  
		virtual std::string name() const throw() {return "xml_string_sensor";}
		///
		virtual std::string description() const throw() {return "A sensor which responds to string values";}
		///@}
	}; 
	DECLARE_HANDLES(XMLStringSensor);
};

#endif 
#endif
 
 /*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/xml_sensor.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------*/


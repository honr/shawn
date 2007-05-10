/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_NODE_SENSOR_H
#define __SHAWN_APPS_NODE_SENSOR_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "apps/reading/sensor.h"

#include <vector>

namespace reading
{
	///	NodeSensor class
	/** This class is the superclass for all node sensors. These sensors stick to a node and do not have a sensor range or a box where they could deliver correct values about the environment. They monitor their carrier.*/
	template <typename T>
	class NodeSensor : public Sensor
	{
	private:
		shawn::Node& n_;
	public:
		typedef T ValueType;
		///
		typedef std::pair<NodeSensorChangedHandler*, shawn::Node*> NodeSensorChangedHandlerPair;
		typedef std::vector<NodeSensorChangedHandlerPair> NodeSensorChangedHandlerList;
		///
		NodeSensorChangedHandlerList nschl_;
		///
		///@name construction / destruction
		///@{
		NodeSensor(shawn::Node& n) : n_ (n)
		{}
		///
		virtual ~NodeSensor()
		{}
		///@}
		///@name class own methods
		///@{
		///	\return current value relating the given node
		/** This methods searches for the nodes value at a certain time.*/
		virtual ValueType value( const shawn::Node& n ) const throw() =0;
		///
		///@name node sensor changed handler related stuff
		///@{
		/**	adds a node sensor changed handler (listener) to the current sensor */
		virtual void add_node_sensor_changed_handler(NodeSensorChangedHandler* nsch, shawn::Node* components_node) throw()
		{
			bool pair_exists = false;
			for(NodeSensorChangedHandlerList::iterator it = nschl_.begin(); it != nschl_.end(); ++it)
			{	
				if(nsch == it->first && components_node == it->second )
				{ 
					TRACE("NODE_SENSOR: allready registered");
					pair_exists = true;
				}
			}
			if( !pair_exists )
			{
				//variable which contains the passed parameters in a struct <pair> NodeSensorChangedHandlerPair
				NodeSensorChangedHandlerPair nsp = std::make_pair(nsch, components_node);
				nschl_.push_back(nsp);
			}
		};
		///
		/**	removes a node sensor changed handler (listener) from the current sensor */
		virtual void remove_node_sensor_changed_handler(NodeSensorChangedHandler* nsch, shawn::Node* components_node) throw()
		{
			TRACE("NODE_SENSOR: removing listener");
			for(NodeSensorChangedHandlerList::iterator it = nschl_.begin(); it != nschl_.end(); ++it)
			{
				if( nsch == it->first && components_node == it->second ) 
				{
				        nschl_.erase(it);
					break;
				}
			}
		};
	};
	// ----------------------------------------------------------------------
	// ----------------------------------------------------------------------
	///Node Integer sensor
	class NodeIntegerSensor : public NodeSensor<int>
	{
	public:
		///@name construction / destruction
		///@{
		NodeIntegerSensor(shawn::Node& n): NodeSensor<int>(n){}
		///
		virtual ~NodeIntegerSensor(){}
		///@}
		///@name keeper managed
		///@{
		virtual std::string name() const throw() {return "node_integer_sensor";}
		///
		virtual std::string description() const throw() {return "A node sensor which responds to integer values";}
		///@}
	};
	DECLARE_HANDLES(NodeIntegerSensor); 
	// ----------------------------------------------------------------------
	// ----------------------------------------------------------------------
	/// Node Double sensor
	class NodeDoubleSensor : public NodeSensor<double>
	{
	public:
		///@name construction / destruction
		///@{
		NodeDoubleSensor(shawn::Node& n): NodeSensor<double>(n){}
		///
		virtual ~NodeDoubleSensor(){}
		///@}
		///@name keeper managed
		///@{
		virtual std::string name() const throw() {return "node_double_sensor";}
		///
		virtual std::string description() const throw() {return "A node sensor which responds to double values";}
		///@}
	};
	DECLARE_HANDLES(NodeDoubleSensor); 
	// ----------------------------------------------------------------------
	// ----------------------------------------------------------------------
	/// Node Bool sensor
	class NodeBoolSensor : public NodeSensor<bool>
	{
	public:
		///@name construction / destruction
		///@{
		NodeBoolSensor(shawn::Node& n): NodeSensor<bool>(n){}
		///
		virtual ~NodeBoolSensor(){}
		///@}
		///@name keeper managed
		///@{
		virtual std::string name() const throw() {return "node_bool_sensor";}
		///
		virtual std::string description() const throw() {return "A node sensor which responds to bool values";}
		///@}
	};
	DECLARE_HANDLES(NodeBoolSensor); 
	// ----------------------------------------------------------------------
	// ----------------------------------------------------------------------
	/// Node String sensor
	class NodeStringSensor : public NodeSensor<std::string>
	{
	public:
		///@name construction / destruction
		///@{
		NodeStringSensor(shawn::Node& n): NodeSensor<std::string>(n){}
		///
		virtual ~NodeStringSensor(){}
		///@}
		///@name keeper managed
		///@{
		virtual std::string name() const throw() {return "node_string_sensor";}
		///
		virtual std::string description() const throw() {return "A node sensor which responds to string values";}
		///@}
	};
	DECLARE_HANDLES(NodeStringSensor); 
}

#endif
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/node_sensor.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------*/


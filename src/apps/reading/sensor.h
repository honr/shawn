/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_SENSOR_H
#define __SHAWN_APPS_SENSOR_H

#include "_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#ifndef READING_TRACE
#define READING_TRACE(x) std::cout<<x<<std::endl;	//show comments
#endif

#include "sys/util/keeper_managed.h"
#include "sys/vec.h"
#include "sys/world.h"
#include "apps/reading/xml_readings_specifications.h"

#include <vector>

namespace reading
{
    class SensorKeeper;
    class SensorChangedHandler;
    class NodeSensorChangedHandler;
       
    DECLARE_HANDLES(Sensor);
    
    	///	Superclass for every type of sensor
	/**	Sensors in general gain information about their environment.
	  *	That can be for instance values like temperature values, sonic values or heart rates.
	  *	Further on, in the derived classes, sensors are divided into simple sensors and node sensors.
	  *	If your sensor should gather information about the environment use the simple sensor type and their derived classes.
	  *	For sensors who should monitor their carrier use the node sensor type and their derived classes.
	  */
	class Sensor : public shawn::KeeperManaged
	{
	public:
		///@name construction / destruction
		///@{
		///
		Sensor();
		///
		virtual ~Sensor();
		///@}
		///@name world membership
		///@{
		/** Sets the world that contains this reading,
		 *  used by World
		 */
		virtual void set_world( shawn::World& )throw();
		///@}
	protected:
		shawn::World* world_;
	};
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	class SensorChangedHandler
	{
	public:
		///@name construction / destruction
		///@{
		SensorChangedHandler(){}
		///
		virtual ~SensorChangedHandler(){}
		///@}
		///@name class own method
		///@{
		/**	This method has to be called from the sensor in order to inform every listener about changes. A listener has to inherit this class. */
		virtual void sensor_changed( Sensor& s, shawn::Vec& v) = 0;
		///@}
	};
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	class NodeSensorChangedHandler
	{
	public:
		///@name construction / destruction
		///@{
		NodeSensorChangedHandler(){}
		///
		virtual ~NodeSensorChangedHandler(){}
		///@}
		///@name class own method
		///@{
		/**	This method has to be called from the sensor in order to inform every listener about changes. A listener has to inherit this class. */
		virtual void node_sensor_changed(Sensor& s) = 0;
		///@}
	};
}

#endif
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/sensor.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------*/


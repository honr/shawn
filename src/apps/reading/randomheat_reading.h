/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_RANDOMHEAT_READING_H
#define __SHAWN_APPS_RANDOMHEAT_READING_H

#include "_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "apps/reading/simple_reading.h"
#include "sys/event_scheduler.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"
 
namespace shawn 
{ 
    class SimulationController;
}
 
namespace reading
{ 
     ///  Example reading generates randomized values
     /**  This class is directly inherited from the superclass IntegerReading 
     *    which is on his side inherited from the superclass Reading, with it 
     *    KeeperManaged. Therefore this class must implement the methods 
     *    name(), description() and value(). 
     *	  Furthermore this reading has to be the one to suffer for the 
     *	  ReadingChangedTask. It is inherited from the EventHandler in order to 
     *    have the chance to initialize an event, done by the init()-method. 
     */
     class RandomHeatReading
         : public IntegerReading, shawn::EventScheduler::EventHandler 
     {
     public:
         ///@name construction/ destruction
         ///@{
         ///
	 RandomHeatReading(shawn::SimulationController&);
         ///
         virtual ~RandomHeatReading();
         ///@}
 
         ///@name basic methods inherited from the superclass KeeperManaged
         ///@{
         /**    Implements the name of this Reading. */
         virtual std::string name( void ) const throw();
         /**    Implements the description of this reading. */ 
         virtual std::string description( void ) const throw();
         ///@}
         
	 ///@name basic methods inherited from SimpleReading
         ///@{
         /**    This method generates random variables and writes them to the output.*/
         virtual ValueType value(const shawn::Vec& v ) const throw();
         ///@}
	 
	 ///@name basic methods inherited from Reading
	 ///@{
	 /// \return the domain of the reading, i.e., the region in which
         /// value() returns meaningful values. It is feasible to query
         /// values outside domain().
	 virtual shawn::Box domain( void ) const throw()
         { 
	 	return shawn::Box::INFINITE_3D_SPACE; 
	 }
	 ///	Here it is used for the initialization of an event
	 ///	For now the code implemented has been factored out
	 virtual void init( void ) throw();
	 ///@}
     
         ///@name basic method inherited from EventHandler
         ///@{
         /**    This method should be exclusively called by the EventScheduler.
          * 	On timeout RandomHeatReading has changed and informs every 
	  *	ReadingChangedHandler, whose box intersects with the box containing 
	  *	new values, that its content has changed.
          */
         void timeout( shawn::EventScheduler&, shawn::EventScheduler::EventHandle, double, shawn::EventScheduler::EventTagHandle& ) throw();
         ///@}
    
    private:     
     	 shawn::SimulationController* sc_;
         shawn::EventScheduler::EventHandle event_handle_;
    }; 
}

#endif 
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/reading/randomheat_reading.h,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: randomheat_reading.h,v $
 *-----------------------------------------------------------------------*/


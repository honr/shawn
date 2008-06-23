/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "traci_processor.h"
#ifdef ENABLE_TRACICLIENT

#include <iostream>
#include "sys/node.h"
#include "traci_processor.h"
#include "traci_controlled_processor.h"
//#include "apps/helloworld/helloworld_message.h"
//#include "sys/node_distance_estimate.h"
//#include "sys/distance_estimates/distance_estimate_keeper.h"
#include "sys/simulation/simulation_controller.h"


using namespace std;
using namespace shawn;

namespace traci
{
   TraCIProcessor::
	   TraCIProcessor() : disabled_(false)
   {}
   // ----------------------------------------------------------------------
   TraCIProcessor::
   ~TraCIProcessor()
   {}
   // ----------------------------------------------------------------------
   void
   TraCIProcessor::
   boot( void )
      throw()
   {
   }
   // ----------------------------------------------------------------------
   bool
   TraCIProcessor::
   process_message( const ConstMessageHandle& ) 
      throw()
   {
      return true;
   }
   // ----------------------------------------------------------------------
   void
   TraCIProcessor::
   work( void )
      throw()
   { 
	   bool to = 
		   owner().world().simulation_controller().environment().
		   optional_bool_param("TraCI_output",false);
	   if (to){
		   cout << "---" << endl;
		   cout << "ID: " << owner().id() << endl;
		   cout << "x: " << owner().real_position().x() << endl;
		   cout << "y: " << owner().real_position().y() << endl;
		   cout << "---" << endl;
	   }
   }

   void 
   TraCIProcessor::
   disable_node() 
	   throw()
   {
		for (shawn::Node::ProcessorList::const_iterator it = owner().processors().begin(); 
			it != owner().processors().end(); ++it){
				if (!dynamic_cast<const traci::TraCIProcessor*>(it->get())){
					traci::TraCIControlledProcessor * tcp = dynamic_cast<traci::TraCIControlledProcessor*>(it->get());
					// Processors must be TraCI controllable using TraCI
					assert(tcp);
					tcp->set_state( Inactive );
				}
		}
		disabled_ = true;
   }

   void 
   TraCIProcessor::
   enable_node() 
	   throw()
   {
		for (shawn::Node::ProcessorList::const_iterator it = owner().processors().begin(); 
			it != owner().processors().end(); ++it){
				if (!dynamic_cast<const traci::TraCIProcessor*>(it->get())){
					traci::TraCIControlledProcessor * tcp = dynamic_cast<traci::TraCIControlledProcessor*>(it->get());
					// Processors must be TraCI controllable using TraCI
					assert(tcp);
					tcp->set_state( Active );
				}
		}
		disabled_ = false;
   }

   bool
	   TraCIProcessor::
	   disabled()
	   throw()
   {
	   return disabled_;
   }
}
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/helloworld/helloworld_processor.cpp,v $
 * Version $Revision: 117 $
 * Date    $Date: 2008-01-11 13:52:33 +0100 (Fr, 11 Jan 2008) $
 *-----------------------------------------------------------------------
 * $Log: helloworld_processor.cpp,v $
 *-----------------------------------------------------------------------*/

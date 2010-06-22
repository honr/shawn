/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/examples/wiseml_example_processor.h"
#ifdef ENABLE_WISEML
#ifdef ENABLE_EXAMPLES

#include "apps/examples/processor/helloworld_processor.h"
#include "apps/examples/processor/helloworld_message.h"
#include "sys/node.h"
#include "apps/wiseml/sensors/wiseml_string_sensor_factory.h"
#include "sys/simulation/simulation_controller.h"
#include "apps/wiseml/writer/wiseml_data_keeper.h"
#include "apps/wiseml/writer/wiseml_trace_collector.h"
#include <iostream>


using namespace std;
using namespace shawn;

namespace wiseml
{
   WisemlExampleProcessor::
   WisemlExampleProcessor()
   :  old_value_(""),
      some_sensor_(NULL)
   {
   }
   // ----------------------------------------------------------------------
   WisemlExampleProcessor::
   ~WisemlExampleProcessor()
   {}
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   void
   WisemlExampleProcessor::
   boot( void )
      throw()
   {
      last_time_of_receive_ = simulation_round();
      send( new helloworld::HelloworldMessage );
      
      sim_controller_ = &owner_w().world_w().simulation_controller_w();

      some_sensor_ = get_string_sensor("battery");
      old_value_ = some_sensor_->value();

   }
   // ----------------------------------------------------------------------
   void
   WisemlExampleProcessor::
   work( void )
      throw()
   {
      string cur_value = some_sensor_->value();
      if(cur_value != old_value_)
      {
         old_value_ = cur_value;
         cout << owner().label() << ":Battery = " << cur_value << endl;
         value_changed("battery");
      }
   }
   // ----------------------------------------------------------------------
   WisemlStringSensor* 
   WisemlExampleProcessor::get_string_sensor(std::string capability)
   {
      sensor_keeper_ = sim_controller_->
         keeper_by_name_w<reading::SensorKeeper>("SensorKeeper");

      reading::SensorFactory *factory = 
         sensor_keeper_->find_w("wiseml_string_sensor").get();

      WisemlStringSensorFactory *string_factory = 
         dynamic_cast<WisemlStringSensorFactory*>(factory);

      WisemlStringSensor* sensor = 
         string_factory->create(capability, owner_w());

      return sensor;
   }
   // ----------------------------------------------------------------------
   void
   WisemlExampleProcessor::value_changed(std::string capability)
   {
      WisemlDataKeeper *keeper = 
         sim_controller_->keeper_by_name_w<WisemlDataKeeper>(
         "wiseml_data_keeper");
      if(keeper != NULL)
      {
         try
         {
            WisemlTraceCollector &trace = keeper->trace("example_trace");
            trace.capability_value(owner().label(), capability, 
               "" + old_value_);
         }
         catch(std::runtime_error er)
         {
         }
      }
   }
   // ----------------------------------------------------------------------
}
#endif
#endif

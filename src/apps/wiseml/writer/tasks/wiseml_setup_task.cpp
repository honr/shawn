/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/writer/tasks/wiseml_setup_task.h"
#ifdef ENABLE_WISEML
#include <iostream>
namespace wiseml
{
   WisemlSetupTask::WisemlSetupTask()
   {
   }
   // ----------------------------------------------------------------------
   WisemlSetupTask::~WisemlSetupTask()
   {
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   std::string WisemlSetupTask::name() const throw()
   {
      return "wiseml_setup";
   }
   // ----------------------------------------------------------------------
   std::string WisemlSetupTask::description() const throw()
   {
      return "Gathers data for the setup section of a WiseML file.";
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   void WisemlSetupTask::run(SimulationController &sc) throw()
   {
      WisemlDataKeeper *keeper = data_keeper(sc);
      WisemlSetupCollector &setup = keeper->setup();

      //Origin:
      double ox = sc.environment().optional_double_param("x", 0.0);
      double oy = sc.environment().optional_double_param("y", 0.0);
      double oz = sc.environment().optional_double_param("z", 0.0);
      double op = sc.environment().optional_double_param("phi", 0.0);
      double ot = sc.environment().optional_double_param("theta", 0.0);
      shawn::Vec origin(ox,oy,oz);
      setup.set_origin(origin, op, ot);

      //Timeinfo:
      std::string start = sc.environment().optional_string_param(
         "start", generate_timestring());
      setup.set_timeinfo_start(start);

      //Interpolation:
      setup.set_interpolation(sc.environment().optional_string_param(
         "interpolation", ""));

      //Description:
      std::string desc = sc.environment().optional_string_param(
         "description", "");
      int first_ul = desc.find_first_of('_');
      while(first_ul != std::string::npos)
      {
         desc = desc.replace(first_ul, 1, " ");
         first_ul = desc.find_first_of('_');
      }
      setup.set_description(desc);

      setup.gather_topology();
   }
   
   // ----------------------------------------------------------------------
   std::string WisemlSetupTask::generate_timestring()
   {
      std::stringstream wml;

      //Current time
      time_t t = time(NULL);
      tm *ts = localtime(&t);

      
      wml << ts->tm_year + 1900 << "-" ;  //year
      if(ts->tm_mon + 1 < 10)
         wml << "0";
      wml << ts->tm_mon + 1 << "-";       //month
      if(ts->tm_mday < 10)
         wml << "0";
      wml << ts->tm_mday << "T";          //day
      if(ts->tm_hour < 10)
         wml << "0";
      wml << ts->tm_hour << ":";          //hour
      if(ts->tm_min < 10)
         wml << "0";
      wml << ts->tm_min << ":";           //minute
      if(ts->tm_sec < 10)
         wml << "0";
      wml << ts->tm_sec << "Z";           //second

      return wml.str();
   }
   // ----------------------------------------------------------------------
   WisemlDataKeeper* WisemlSetupTask::data_keeper(SimulationController &sc)
   {
      WisemlDataKeeper *keeper = sc.keeper_by_name_w<WisemlDataKeeper>(
         "wiseml_data_keeper");
      if(keeper == NULL)
      {
         sc.add_keeper(keeper = new WisemlDataKeeper(sc));
      }

      return keeper;
   }
}
#endif

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/writer/wiseml_trace_collector.h"
#ifdef ENABLE_WISEML
namespace wiseml
{
   WisemlTraceCollector::
      WisemlTraceCollector(shawn::SimulationController &sc,
      std::string id) 
      :  WisemlDataCollector(sc),
         id_(id)
   {}

   WisemlTraceCollector::~WisemlTraceCollector(){}

   std::string WisemlTraceCollector::name()
      const throw()
   {
      return id_;
   }

   std::string WisemlTraceCollector::description()
      const throw()
   {
      return "Trace section data";
   }
}
#endif

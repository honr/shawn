/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/writer/wiseml_writer_task.h"
#ifdef ENABLE_WISEML
#include <iostream>
#include <fstream>
#include <apps/wiseml/writer/wiseml_generator.h>
namespace wiseml
{
   WisemlWriterTask::WisemlWriterTask()
   {
   }
   // ----------------------------------------------------------------------
   WisemlWriterTask::~WisemlWriterTask()
   {
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   std::string WisemlWriterTask::name() const throw()
   {
      return "wiseml_writer";
   }
   // ----------------------------------------------------------------------
   std::string WisemlWriterTask::description() const throw()
   {
      return "A task for generating WiseML files of the current topology.";
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   void WisemlWriterTask::run(SimulationController &sc) throw()
   {
      std::string filename = sc.environment().optional_string_param("filename", "simulation.wiseml");
      std::ofstream file;
      file.open(filename.c_str());

      WisemlGenerator wml_gen(sc);
      file << wml_gen.generate_wiseml();
      file.close();
   }
}
#endif

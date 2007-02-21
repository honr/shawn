/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/misc/localization/localization_processor_type.h"

namespace shawn
{

   LocalizationProcessorType::
   LocalizationProcessorType()
      : proc_type_( unknown )
   {}
   // ----------------------------------------------------------------------
   LocalizationProcessorType::
   ~LocalizationProcessorType()
   {}
   // ----------------------------------------------------------------------
   void
   LocalizationProcessorType::
   set_proc_type( const ProcessorType& pt )
      throw()
   {
      proc_type_ = pt;
   }
   // ----------------------------------------------------------------------
   const LocalizationProcessorType::ProcessorType&
   LocalizationProcessorType::
   proc_type( void )
      const throw()
   {
      return proc_type_;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationProcessorType::
   is_anchor( void )
      const throw()
   {
      return proc_type_ == anchor;
   }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/localization/localization_processor_type.cpp,v $
 * Version $Revision: 1.3 $
 * Date    $Date: 2005/08/05 10:00:39 $
 *-----------------------------------------------------------------------
 * $Log: localization_processor_type.cpp,v $
 *-----------------------------------------------------------------------*/

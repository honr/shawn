/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/messages/refinement/localization_iter_lateration_messages.h"


namespace localization
{

   LocalizationIterLaterationMessage::
   LocalizationIterLaterationMessage( double confidence )
      : confidence_( confidence )
   {}
   // ----------------------------------------------------------------------
   LocalizationIterLaterationMessage::
   LocalizationIterLaterationMessage( const LocalizationIterLaterationMessage& lilm )
      : confidence_( lilm.confidence() )
   {}
   // ----------------------------------------------------------------------
   LocalizationIterLaterationMessage::
   ~LocalizationIterLaterationMessage()
   {}
   // ----------------------------------------------------------------------
   double
   LocalizationIterLaterationMessage::
   confidence( void )
      const throw()
   {
      return confidence_;
   }

   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------

   LocalizationIterLaterationSoundMessage::
   LocalizationIterLaterationSoundMessage()
   {}
   // ----------------------------------------------------------------------
   LocalizationIterLaterationSoundMessage::
   LocalizationIterLaterationSoundMessage( const LocalizationIterLaterationSoundMessage&  )
   {}
   // ----------------------------------------------------------------------
   LocalizationIterLaterationSoundMessage::
   ~LocalizationIterLaterationSoundMessage()
   {}

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/messages/refinement/localization_iter_lateration_messages.cpp,v $
 * Version $Revision: 1.3 $
 * Date    $Date: 2005/08/05 10:00:31 $
 *-----------------------------------------------------------------------
 * $Log: localization_iter_lateration_messages.cpp,v $
 *-----------------------------------------------------------------------*/

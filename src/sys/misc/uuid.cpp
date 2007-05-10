/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <ctime>
#include "sys/misc/uuid.h"

namespace shawn
{
   UUIDGenerator*    UUIDGenerator::instance_ ( NULL );
   std::string       UUIDGenerator::alphabet_
   ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
   unsigned long int UUIDGenerator::alphabet_length_ ( UUIDGenerator::alphabet_.size() );


   // ----------------------------------------------------------------------
   std::string
   UUIDGenerator::
   uuid( void )
      throw()
   {
      if( instance_ == NULL )
         instance_ = new UUIDGenerator;
      return instance_->generate();
   }
   // ----------------------------------------------------------------------
   UUIDGenerator::
   UUIDGenerator()
   {
      current_=0;
      prefix_ = ulong2string((unsigned long int)(time(NULL)));
   }
   // ----------------------------------------------------------------------
   UUIDGenerator::
   ~UUIDGenerator()
   {}
   // ----------------------------------------------------------------------
   std::string
   UUIDGenerator::
   ulong2string( unsigned long int val )
      throw()
   {
      std::string res("");
      do
         {
            res += alphabet_[ val % alphabet_length_ ];
            val /= alphabet_length_;
         }
      while( val>0 );
      return res;
   }
   // ----------------------------------------------------------------------
   std::string
   UUIDGenerator::
   generate( void )
      throw()
   {
      return prefix_ + std::string("-") + ulong2string(current_++);
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/uuid.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: uuid.cpp,v $
 *-----------------------------------------------------------------------*/

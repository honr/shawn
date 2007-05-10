/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "helloworld_processor.h"
#ifdef ENABLE_HELLOWORLD

#include <iostream>
#include "sys/node.h"
#include "apps/helloworld/helloworld_processor.h"
#include "apps/helloworld/helloworld_message.h"

using namespace std;
using namespace shawn;

namespace helloworld
{
   HelloworldProcessor::
   HelloworldProcessor()
      : last_time_of_receive_( 0 )
   {}
   // ----------------------------------------------------------------------
   HelloworldProcessor::
   ~HelloworldProcessor()
   {}
   // ----------------------------------------------------------------------
   void
   HelloworldProcessor::
   boot( void )
      throw()
   {
      last_time_of_receive_ = simulation_round();
      send( new HelloworldMessage );
   }
   // ----------------------------------------------------------------------
   bool
   HelloworldProcessor::
   process_message( const ConstMessageHandle& mh ) 
      throw()
   {
      const HelloworldMessage* hmsg =
         dynamic_cast<const HelloworldMessage*>
         ( mh.get() );
      if( hmsg != NULL )
         {
            last_time_of_receive_=simulation_round();
            neighbours_.insert( &hmsg->source() );
            cout << "ID '" << owner().label() << "' GOT HELLO FROM '"
                 << hmsg->source().label() << "'" << endl;
            return true;
         }

      return Processor::process_message( mh );
   }
   // ----------------------------------------------------------------------
   void
   HelloworldProcessor::
   work( void )
      throw()
   {
      if( simulation_round()-last_time_of_receive_ > 4 )
         {
            cout << "ID " << id()
                 << " DONE: "
                 << (unsigned int)neighbours_.size()
                 << " neighbours: ";
            
            for( std::set<const Node*>::const_iterator
                    it    = neighbours_.begin(),
                    first = it,
                    endit = neighbours_.end();
                 it != endit; ++it )
               {
                  if( it != first )
                     cout << ", ";
                  cout << "'"<<(**it).label()<<"'";
               }
            cout << endl;
            set_state( Inactive );
         }
   }






}
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/helloworld/helloworld_processor.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: helloworld_processor.cpp,v $
 *-----------------------------------------------------------------------*/

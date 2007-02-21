/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/constants.h"
#include "sys/event_scheduler.h"

namespace shawn
{
   const EventScheduler::EventHandle EventScheduler::NO_EVENT = NULL;

   EventScheduler::EventInfo::
   EventInfo( EventHandler& h, 
              double t,
              const EventTagHandle& tag )
      : handler_ ( &h ),
        time_    ( t ),
        dead_    ( false ),
        tag_     ( tag )
   {}
   // ----------------------------------------------------------------------
   EventScheduler::EventInfo::
   ~EventInfo()
   {}
   // ----------------------------------------------------------------------
   bool
   EventScheduler::EventInfo::
   operator < ( const EventScheduler::EventInfo& ei )
   {
      if( time_ < ei.time_-EPSILON )
         return true;
      else if( time_ < ei.time_+EPSILON )
         return (void*)this < (void*)(&ei);
      else
         return false;
   }
   // ----------------------------------------------------------------------
   double 
   EventScheduler::EventInfo::
   time() 
   const throw()
   {
	return time_;
   }










   EventScheduler::EventHandler::
   EventHandler()
   {}
   // ----------------------------------------------------------------------
   EventScheduler::EventHandler::
   ~EventHandler()
   {}











   EventScheduler::
   EventScheduler()
      : time_ ( 0.0 )
   {}
   // ----------------------------------------------------------------------
   EventScheduler::
   ~EventScheduler()
   {}
   // ----------------------------------------------------------------------
   EventScheduler::EventHandle
   EventScheduler::
   new_event( EventScheduler::EventHandler& h,
              double                        t,
              const EventScheduler::EventTagHandle& eth )
      throw()
   {
      assert( t >= current_time() );
      EventHandle eh = new EventInfo( h, t, eth );

      std::pair<EventSet::iterator,bool> 
         worked = events_.insert(eh);
      assert( worked.second );
      eh->pos_ = worked.first;
      return eh;
   }
   // ----------------------------------------------------------------------
   void
   EventScheduler::
   delete_event( EventScheduler::EventHandle eh ) 
      throw()
   {
      assert( !eh->dead_ ); // must not delete_event in it's timeout
      assert( eh != NULL );
      assert( events_.find(eh) != events_.end() );
      events_.erase( eh->pos_ );
      delete eh;
   }
   // ----------------------------------------------------------------------
   void 
   EventScheduler::
   move_event( EventHandle eh, 
               double      newt )
      throw()
   {
      assert( newt >= current_time() );
      assert( eh != NULL );
      assert( events_.find(eh) != events_.end() );
      events_.erase( eh->pos_ );
      eh->time_ = newt;
      std::pair<EventSet::iterator,bool> 
         worked = events_.insert(eh);
      assert( worked.second );
      eh->pos_ = worked.first;
      eh->dead_ = false;
   }
   // ----------------------------------------------------------------------
   double
   EventScheduler::
   current_time( void )
      const throw()
   {
      return time_;
   }
   // ----------------------------------------------------------------------
   void
   EventScheduler::
   playback( double stop_time )
      throw()
   {
      assert( stop_time >= current_time() );

      while( 1 )
         {
            if( empty() )
               {
                  time_ = stop_time;
                  return;
               }
         
            double net = next_event_time();
            if( net < stop_time )
               {
                  time_ = net;
                  EventScheduler::EventHandle eh = front_w();
                  assert( eh->pos_ == events_.begin() );
                  assert( net == eh->time_ );
                  assert( !eh->dead_ );
                  eh->dead_ = true;

                  eh->handler_->timeout( *this, eh, net, eh->tag_ );
               
                  if( eh->dead_ )
                     {
                        events_.erase(eh);
                        delete eh;
                     }
               }
            else
               {
                  time_ = stop_time;
                  return;
               }
         }
   }
   // ----------------------------------------------------------------------
   double
   EventScheduler::
   next_event_time( void )
      const throw()
   {
      assert( !empty() );
      return front()->time_;
   }
   // ----------------------------------------------------------------------
   bool
   EventScheduler::
   empty( void )
      const throw()
   {
      return events_.empty();
   }
   // ----------------------------------------------------------------------
   EventScheduler::EventHandle 
   EventScheduler::
   front_w( void )
      throw()
   {
      assert( !empty() );
      return *events_.begin();
   }
   // ----------------------------------------------------------------------
   const EventScheduler::EventHandle
   EventScheduler::
   front( void )
      const throw()
   {
      assert( !empty() );
      return *events_.begin();
   }
   // ----------------------------------------------------------------------
   void
   EventScheduler::
   clear( double new_time )
      throw()
   {
      events_.clear();
      time_=new_time;
   }




}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/event_scheduler.cpp,v $
 * Version $Revision: 1.4 $
 * Date    $Date: 2007/01/18 13:17:42 $
 *-----------------------------------------------------------------------
 * $Log: event_scheduler.cpp,v $
 *-----------------------------------------------------------------------*/

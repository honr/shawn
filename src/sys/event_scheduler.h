/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_EVENT_SCHEDULER_H
#define __SHAWN_SYS_EVENT_SCHEDULER_H

#include "sys/util/refcnt_pointable.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/defutils.h"

#include <set>

namespace shawn
{

class EventScheduler
{
public:
   
   ///@name Local Types
   ///@{
   class EventHandler;
   class EventInfo;
   ///
   typedef EventInfo* EventHandle;
   ///
   static const EventHandle NO_EVENT;
   ///
   struct EventSorter
   {
      bool operator() ( const EventHandle& e1, const EventHandle& e2 ) const throw()
      { return (*e1) < (*e2); }
   };
   ///
   typedef
   std::set<EventHandle,EventSorter> 
   EventSet;
   ///
   DECLARE_HANDLES(EventTag);
   /// Stuff to derive from, thereby adding tagged info to events
   class EventTag
      : public RefcntPointable
   {
   public:
      EventTag();
      virtual ~EventTag();
   };
   ///   
   class EventInfo
   {
      friend class EventScheduler;
   public:
      bool operator < ( const EventInfo& ei );
      double time() const throw(); 
   private:
      EventInfo( EventHandler&, double, const EventTagHandle& );
      ~EventInfo();
      EventHandler*      handler_;
      double             time_;
      EventSet::iterator pos_;
      bool               dead_;
      EventTagHandle     tag_;
   };
   /// Interface for event-handling classes
   class EventHandler
   {
   public:
      ///
      EventHandler();
      ///
      virtual ~EventHandler();
      /// Main interface method. Called when a scheduled event happens,
      /// that is, timeouts.
      /// EventHandle gets automagically deleted after handler returns,
      /// unless move_event() is used.
      /// Deleting the event during it's handling is a no-no.
      virtual void timeout( EventScheduler&, EventHandle, double, EventTagHandle& ) throw() = 0;
   };
   ///@}

   ///@name Construction / Destruction
   ///@{
   ///
   EventScheduler();
   ///
   virtual ~EventScheduler();
   ///@}


   ///@name Event Management
   ///@{
   /** Creates a new event that timeouts at time \a t, at which time
    * \a eh ->timeout() will be called.
    * \a tag may be NULL or anything the handler might find useful
    * \returns a handle (read: ID) by which the new event is identified
    */
   EventHandle new_event( EventHandler& eh, double t, const EventTagHandle& eth ) throw();
   /** Removes the specified event. Must not be called from within
    * timeout() (at least for the timeouting event)
    */
   void delete_event( EventHandle ) throw();
   /** Re-schedules the event for a different time, abandoning the current one
    */
   void move_event( EventHandle, double ) throw();
   ///@}


   ///@name Info Access
   ///@{
   ///
   double current_time( void ) const throw();
   ///
   double next_event_time( void ) const throw();
   ///
   bool empty( void ) const throw();
   ///
   EventHandle front_w( void ) throw();
   ///
   const EventHandle front( void ) const throw();
   ///@}

   ///@name Running
   ///@{
   ///
   void playback( double stop_time ) throw();
   ///
   void clear( double new_time = 0.0 ) throw();
   ///@}

private:
   EventSet events_;
   double time_;
};



}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/event_scheduler.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: event_scheduler.h,v $
 *-----------------------------------------------------------------------*/

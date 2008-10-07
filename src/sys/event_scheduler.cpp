/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/constants.h"
#include "sys/event_scheduler.h"
#include "sys/util/defutils.h"
#include <iostream>

namespace shawn
{
   const EventScheduler::EventHandle EventScheduler::NO_EVENT = NULL;

	// ----------------------------------------------------------------------
	EventScheduler::EventInfo::
		EventInfo( EventHandler& h, double t, const EventTagHandle& tag )
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
		if (EQDOUBLE(time_, ei.time_))
			return (void*)this < (void*)(&ei);
		else
			return time_ < ei.time_;
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





	// ----------------------------------------------------------------------
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
		new_event( EventScheduler::EventHandler& h, double t, const EventScheduler::EventTagHandle& eth )
		throw()
	{
		assert( t >= current_time() );
		EventHandle eh = new EventInfo( h, t, eth );

		std::pair<EventSet::iterator,bool> worked = events_.insert(eh);
		if( !worked.second )
		{
			std::cout << "ASSERT OCCURED: " << std::endl;
			std::cout << "this ->time_ " << eh->time_ << ", ptr=" << (void*)eh << std::endl;
			std::cout << "(*it)->time_ " << (*(worked.first))->time_ << ", ptr=" << (void*)(*(worked.first)) << std::endl;
		}
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

		bool event_found = false;
		if (events_.find(eh) == events_.end())
		{
			for (EventSet::iterator it = events_.begin(); it != events_.end(); ++it)
			{
				if ( eh->pos_ == it )
				{
					event_found = true;
					break;
				}
			}
		}
		else
		{
			event_found = true;
		}

		assert( event_found );
		events_.erase( eh->pos_ );
		delete eh;
	}

	// ----------------------------------------------------------------------
	void
		EventScheduler::
		move_event( EventHandle eh, double newt )
		throw()
	{
		assert( newt >= current_time() );
		assert( eh != NULL );

		bool event_found = false;
		if (events_.find(eh) == events_.end())
		{
			for (EventSet::iterator it = events_.begin(); it != events_.end(); ++it)
			{
				if ( eh->pos_ == it )
				{
					event_found = true;
					break;
				}
			}
		}
		else
		{
			event_found = true;
		}

		assert( event_found );
		events_.erase( eh->pos_ );
		eh->time_ = newt;
		std::pair<EventSet::iterator,bool> worked = events_.insert(eh);
		assert( worked.second );
		eh->pos_ = worked.first;
		eh->dead_ = false;
	}

/*
    // ----------------------------------------------------------------------
   const EventScheduler::EventHandle 
   EventScheduler::
   find_event( EventHandler& eh ) 
   const
   {
      for (EventSet::const_iterator it = events_.begin(); it != events_.end(); ++it){
         if ( &eh == (*it)->handler_ ){
            return *it;
         }
      }
      return NULL;
   }
*/
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
				#ifdef SHAWN_EV_SCHED_ENABLE_RATE_ADAPTATION
					sys_time_.sleep( int(1000.0 * (stop_time - current_time())) );
					sys_time_.touch();
				#endif
				time_ = stop_time;
				return;
			}

			double net = next_event_time();
			if( net < stop_time )
			{
				#ifdef SHAWN_EV_SCHED_ENABLE_RATE_ADAPTATION
					double real_duration = double(sys_time_.ms_since_last_touch());
					double virt_duration = 1000.0 * (net - time_);

					if( real_duration < virt_duration )
						sys_time_.sleep( int(virt_duration - real_duration) );
					sys_time_.touch();
				#endif

				time_ = net;
				EventScheduler::EventHandle eh = front_w();

				assert( eh );
				assert( eh->pos_ == events_.begin() );
				assert( net == eh->time_ );
				assert( !eh->dead_ );

				eh->dead_ = true;
				eh->handler_->timeout( *this, eh, net, eh->tag_ );

				if( eh->dead_ )
				{
					events_.erase( eh->pos_ );
					delete eh;
				}
			}
			else
			{
				#ifdef SHAWN_EV_SCHED_ENABLE_RATE_ADAPTATION
					sys_time_.sleep( int(1000.0 * (stop_time - current_time())) );
					sys_time_.touch();
				#endif
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
		for( EventSet::iterator it = events_.begin(); it != events_.end(); ++it )
			if(*it)
				delete *it;

		events_.clear();
		time_=new_time;
	}

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/event_scheduler.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: event_scheduler.cpp,v $
 *-----------------------------------------------------------------------*/

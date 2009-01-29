/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_VIS_TASK_REFRESH_LIVEVIEW_EVENT_H
#define __SHAWN_VIS_TASK_REFRESH_LIVEVIEW_EVENT_H
#include "../buildfiles/_apps_enable_cmake.h"

#ifdef ENABLE_VIS
#include "apps/vis/base/vis_task.h"
#include "sys/event_scheduler.h"

#ifdef HAVE_BOOST
#include <boost/thread.hpp>
#endif

namespace vis
{
   /** \brief Refreshes the image output window
    */
   class RefreshLiveviewEvent
      : public shawn::EventScheduler::EventHandler
   {
   public:
      ///@name Constructor/Destructor
	   ///@{
      RefreshLiveviewEvent(double refresh_interval, int min_delay, Visualization &vis);
      virtual ~RefreshLiveviewEvent();
      ///@}

      void timeout(shawn::EventScheduler &es, 
         shawn::EventScheduler::EventHandle eh, double t, 
         shawn::EventScheduler::EventTagHandle &);

   protected:
      ///@name Getter
      ///@{
      /**
       * Gets the visualization object.
       */
      inline Visualization& visualization_w( void ) throw()
      { assert( vis_.is_not_null() ); return *vis_; }
      /**
       * Gets the visualization object (constant).
       */
      inline const Visualization& visualization( void ) const throw()
      { assert( vis_.is_not_null() ); return *vis_; }

      virtual void write_frame()
         throw( std::runtime_error );

   private:
      /// Handle of the visualization instance.
      VisualizationHandle vis_;

      /// Refresh interval (shawn time)
      double refresh_interval_;

      /// Minimal time (real time) between two refresh calls (pauses shawn if needed)
      long min_delay_time_;

#ifdef HAVE_BOOST
      /// Time of last refresh
      boost::xtime last_refresh_;
#endif
   };

}

#endif
#endif
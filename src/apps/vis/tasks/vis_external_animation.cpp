/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/tasks/vis_external_animation.h"
#include "apps/vis/writer/vis_writer_factory.h"
#include "apps/vis/writer/vis_writer_keeper.h"
#include <iomanip>
#include <sstream>
#include <math.h>

#define VIS_HAVE_ETA

#ifdef VIS_HAVE_ETA
#include <ctime>
#endif


namespace vis
{

   ExternalAnimationTask::
   ExternalAnimationTask()
   {}
   // ----------------------------------------------------------------------
   ExternalAnimationTask::
   ~ExternalAnimationTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   ExternalAnimationTask::
   name( void )
      const throw()
   { return "vis_external_animation"; }
   // ----------------------------------------------------------------------
   std::string
   ExternalAnimationTask::
   description( void )
      const throw()
   { return "XXX"; }
   // ----------------------------------------------------------------------
   void
   ExternalAnimationTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      VisualizationTask::run(sc);

      double start = sc.environment().optional_double_param("start",0.0);
      double end = sc.environment().optional_double_param("end",0.0);
      double fps = sc.environment().optional_double_param("fps",0.0);
      std::string fn = sc.environment().optional_string_param("file_base","anim");
      int dr = sc.environment().optional_int_param("draft",0);

	  WriterFactoryHandle wfh = sc.keeper_by_name_w<WriterKeeper>("WriterKeeper")
		  ->find_w(sc.environment().optional_string_param("writer", "pdf"));
	  Writer* wr = wfh->create();

      wr->set_draft(dr);
      wr->pre_write( visualization(),
                     fn,
                     true );
      int lastframe=int( floor( (end-start-EPSILON)*fps ) );

#ifdef VIS_HAVE_ETA
      time_t start_time = time(NULL);
      std::string eta_str("");
#endif
      for( int frame = 0; frame<=lastframe; ++frame )
         {
            double now=start+(double(frame)/fps);

            double perc = double(frame)/double(lastframe);
			perc = int((perc*1000)+.5)/10.0;

#ifdef VIS_HAVE_ETA
            time_t elapsed = time(NULL)-start_time;
            if( elapsed>3 && frame>0 )
               { 
                  time_t eta=int(double(elapsed)*double(lastframe-frame)/double(frame));
                  std::ostringstream oss;
                  oss << " / Est " << (eta/3600) << ":" << std::setw(2) << std::setfill('0') << ((eta/60)%60) << ":" << std::setfill('0') << std::setw(2) << eta%60;
                  eta_str=oss.str();
               }
#endif

#ifdef VIS_HAVE_ETA
            INFO( logger(), "writing frame #" << wr->next_frame_number() << " (" << perc <<"%" << eta_str << ")");
#else
            INFO( logger(), "writing frame #" << wr->next_frame_number() << " (" << perc <<"%");
#endif
            wr->write_frame( now );
         }
      wr->post_write();
      delete wr;
   }


}
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/tasks/vis_external_animation.cpp,v $
 * Version $Revision: 1.8 $
 * Date    $Date: 2006/03/15 14:14:02 $
 *-----------------------------------------------------------------------
 * $Log: vis_external_animation.cpp,v $
 * Revision 1.8  2006/03/15 14:14:02  pfister
 * *** empty log message ***
 *
 * Revision 1.7  2006/02/26 12:00:59  ali
 * *** empty log message ***
 *
 * Revision 1.6  2006/02/25 15:21:41  ali
 * *** empty log message ***
 *
 * Revision 1.5  2006/02/22 22:27:24  ali
 * *** empty log message ***
 *
 * Revision 1.4  2006/02/22 09:18:57  ali
 * *** empty log message ***
 *
 * Revision 1.3  2006/02/19 21:34:29  ali
 * *** empty log message ***
 *
 * Revision 1.2  2006/02/01 17:07:29  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/01/31 12:44:00  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/01/29 21:02:01  ali
 * began vis
 *
 *-----------------------------------------------------------------------*/

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/geo/geo_routing.h"
#include "apps/routing/geo/test/geo_postscript.h"

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/vec.h"
#include "sys/world.h"
#include "sys/node.h"

#include <fstream>

#define _MIN_(x,y) ( (x)<(y) ? (x) : (y) )
#define _MAX_(x,y) ( (x)<(y) ? (y) : (x) )

using namespace std;
using namespace shawn;

namespace routing
{
 namespace geo 
 {

      GeoPostscript::
         GeoPostscript(shawn::SimulationController& sc, string filename)
            : outfile_( NULL ),
              picture_dimension_ ( 600 ),
              border_width_ ( 10 )
      {
         page_count_= 1;
         outfile_ = new ofstream(filename.c_str());
   
         if( outfile_->bad() )
            throw std::runtime_error("Specified file invalid");
         
         print_header(sc);
      }
         
      // --------------------------------------------------
      GeoPostscript::
         ~GeoPostscript()
      {
         if( outfile_ != NULL )
            delete outfile_;
      }
   
      // --------------------------------------------------
      void 
         GeoPostscript::
         draw_nodes(shawn::World::const_node_iterator begin, shawn::World::const_node_iterator end)  
         throw()
      {
         //Connectivity lines
         for(shawn::World::const_node_iterator it = begin; it!=end; ++it)
         {
            const shawn::Node& n1 = *it;
            Vec pos1  = pos2ps(node_pos(n1));
            for(shawn::Node::const_adjacency_iterator nit = n1.begin_adjacent_nodes(), nend = n1.end_adjacent_nodes(); nit!=nend; ++nit)
               if( (*nit).id() < n1.id() ) {
                  Vec pos2  = pos2ps(node_pos((*nit)));
                  (*outfile_) << "lightedgestyle " << pos1.x() << " " << pos1.y() << " moveto " << pos2.x() << " " << pos2.y() << " lineto stroke" << endl;
               }
         }
   
         //Nodes
         for(shawn::World::const_node_iterator it = begin; it!=end; ++it)
         {
            const shawn::Node& n1 = *it;
            Vec pos1  = pos2ps(node_pos(n1));
            (*outfile_) << pos1.x() << " " << pos1.y() << " " << "normalnode" << endl;         
         }
         
      }
   
      // --------------------------------------------------
      void 
         GeoPostscript::
         draw_path(std::list<shawn::Node*>::iterator begin, std::list<shawn::Node*>::iterator end,
                                std::string path_linestyle /* = "msgpathstyle" */,
                                bool drawCommRange /* = false */, bool drawEuclidean /* = false */,
                                const shawn::Vec& dest /* = shawn::Vec(0,0,0) */, double comm_range /* = 0.0 */
                                ) 
         throw()
      {
         assert(outfile_ != NULL);
         std::list<shawn::Node*>::iterator destnodeit = end;
         shawn::Node* n1 = NULL, *n2 = NULL, *start = *begin, *destnode = *(--destnodeit);
         
         if(start == NULL || destnode == NULL)
         {
            GEO_DEBUG(2, "GeoPostscript::draw_path: Could not draw path: start["<<start<<"], destnode["<<destnode<<"]");
            return;
         }
         
         Vec posdest  = pos2ps(dest);
         Vec posbeg  = pos2ps(node_pos(*start));
         Vec posdestnode  = pos2ps(node_pos(*destnode));
         double radius = scale_ * comm_range;
   
         //Draw the complete message path
         for(std::list<shawn::Node*>::iterator it = begin; it!=end; ++it)
         {
            n2 = n1;
            n1 = *it;
            assert(n1 != NULL);
            Vec pos1 = pos2ps(node_pos(*n1));
            
            //Draw the euclidean distance radius
            if( drawEuclidean )
               (*outfile_) << "lightlightstyle " << posdest.x() << " " << posdest.y() << " " 
                           << ((pos1 - posdest).euclidean_norm()) << " fas" << endl;
            
            //Draw the communication range
            if( drawCommRange )
               (*outfile_) << "itmstyle " << pos1.x() << " " << pos1.y() << " " << radius <<" fas" << endl;
   
            //We need two points for a line segment
            if( n2 == NULL )
               continue;
            
            //Draw the message path
            Vec pos2  = pos2ps(node_pos(*n2));
            (*outfile_) << path_linestyle << " " << pos1.x() << " " << pos1.y() << " moveto " 
                        << pos2.x() << " " << pos2.y() << " lineto stroke" << endl;
         }
         
      }
      
      // ----------------------------------------------------------------------
      void
         GeoPostscript::
         start_page()
         throw( std::runtime_error )
      {
         assert(outfile_ != NULL);
         ++page_count_;
         (*outfile_) << "%%Page: " << page_count_ << " " << page_count_ << endl;
      }   
      // ----------------------------------------------------------------------
      void
         GeoPostscript::
         end_page()
         throw( std::runtime_error )
      {
         assert(outfile_ != NULL);
         (*outfile_) << "showpage" << endl;
      }
   
      // ----------------------------------------------------------------------
      void
         GeoPostscript::
         custom(std::string cmd) 
         throw()
      {
         (*outfile_) << cmd << endl;
      }
     
      // ----------------------------------------------------------------------
      void
         GeoPostscript::
         print_header( SimulationController& sc)
         throw( std::runtime_error )
      {
         assert(outfile_ != NULL);
         (*outfile_) 
            << "%!PS-Adobe-2.0" << endl
            << "%%BoundingBox: 0 0 " << picture_dimension_ << " " << picture_dimension_ << endl
            << "%%Pages: (atend)" << endl
            << "%%PageOrder: Ascend" << endl
            << "%%EndComments" << endl
         
            << "/normalnode {" << endl
            << "  gsave" << endl
            << "    translate" << endl
            << "    [ ] 0 setdash " << endl
            << "    0 setgray" << endl
            << "    newpath 0 0 1.5 0 360 arc closepath fill" << endl
            << "  grestore" << endl
            << "} bind def" << endl
         
            << "/sourcenode {" << endl
            << "  gsave" << endl
            << "    translate" << endl
            << "    [ ] 0 setdash " << endl
            << "    1 0 0 setrgbcolor" << endl
            << "    newpath 0 0 3 0 360 arc closepath fill" << endl
            << "  grestore" << endl
            << "} bind def" << endl
         
            << "/destnode {" << endl
            << "  gsave" << endl
            << "    translate" << endl
            << "    [ ] 0 setdash " << endl
            << "    0 1 0 setrgbcolor" << endl
            << "    newpath 0 0 3 0 360 arc closepath fill" << endl
            << "  grestore" << endl
            << "} bind def" << endl
            
            << "/destpos {" << endl
            << "  gsave" << endl
            << "    translate" << endl
            << "    [ ] 0 setdash " << endl
            << "    0 1 0 setrgbcolor" << endl
            << "    newpath 0 0 6 0 360 arc closepath fill" << endl
            << "  grestore" << endl
            << "} bind def" << endl
            
            << "/msgpathstyle {" << endl
            << " [ ] 0 setdash " << endl
            << " 1 0.6 0 setrgbcolor" << endl
            << "  1.5 setlinewidth" << endl
            << "} bind def" << endl
            
            << "/edgestyle {" << endl
            << " [ ] 0 setdash " << endl
            << "  .5 setgray" << endl
            << "  1 setlinewidth" << endl
            << "} bind def" << endl
            
            << "/lightedgestyle {" << endl
            << " [ ] 0 setdash " << endl
            << "  .9 setgray" << endl
            << "  0.3 setlinewidth" << endl
            << "} bind def" << endl
   
            << "/lightlightstyle {" << endl
            << " [ ] 0 setdash " << endl
            << "  0 0 0 setrgbcolor" << endl
            << "  0.15 setlinewidth" << endl
            << "} bind def" << endl
   
            << "/itmstyle {" << endl
   //         << " [3] 0 setdash " << endl
            << " 0.1 setlinewidth " << endl 
            << " 1 0.6 0 setrgbcolor" << endl
            << "} bind def" << endl
            
            << "/spstyle {" << endl
            << " 0.5 setlinewidth  " << endl 
            << " [3 3] 0 setdash " << endl 
            << " 1 setlinewidth " << endl 
            << " 1 0 0 setrgbcolor " << endl
            << "} bind def" << endl
            
            << "/faf {" << endl
            << " 0 360 arc fill" << endl 
            << "} bind def" << endl
   
            << "/fas {" << endl
            << " 0 360 arc stroke" << endl 
            << "} bind def" << endl
   
            << "%%Page: 1 1" << endl
            << "1 setlinecap" << endl;
   
         shawn::Vec ur = shawn::Vec(-numeric_limits<double>::max(),-numeric_limits<double>::max(),-numeric_limits<double>::max());
         shawn::Vec ll = shawn::Vec(numeric_limits<double>::max(),numeric_limits<double>::max(),numeric_limits<double>::max());
         
         for(shawn::World::const_node_iterator it = sc.world().begin_nodes(), end = sc.world().end_nodes(); it!=end; ++it)
         {
            const Vec& p = (*it).real_position();
            ll = shawn::Vec( _MIN_(ll.x(),p.x()), _MIN_(ll.y(),p.y()), _MIN_(ll.z(),p.z()) );
            ur = shawn::Vec( _MAX_(ur.x(),p.x()), _MAX_(ur.y(),p.y()), _MAX_(ur.z(),p.z()) );
         }
         
         double wdt= ur.x() - ll.x();
         double hgt= ur.y() - ll.y();
         scale_=(picture_dimension_-2.0*border_width_)/wdt;
         
         double scy=(picture_dimension_-2.0*border_width_)/hgt;
         
         if( scy < scale_ ) 
            scale_=scy;
   
         ps_x0_ = border_width_ - (scale_*ll.x());
         ps_y0_ = border_width_ - (scale_*ll.y());
      }
      
      // ----------------------------------------------------------------------
      Vec
         GeoPostscript::
         pos2ps( const Vec& v ) 
         const throw()
      {
         return Vec( (v.x()*scale_)+ps_x0_, (v.y()*scale_)+ps_y0_ );
      }
   
      // ----------------------------------------------------------------------
      double
         GeoPostscript::
         scale(double d) 
         const throw()
      {
         return d * scale_;
      }

   }
}
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/routing/geo/test/geo_postscript.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: geo_postscript.cpp,v $
 *-----------------------------------------------------------------------*/

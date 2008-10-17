/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/base/visualization.h"
#include <sstream>
#include <algorithm>

namespace vis
{

   Visualization::
   Visualization( const std::string& n )
      : name_ ( n )
   {}
   // ----------------------------------------------------------------------
   Visualization::
   ~Visualization()
   {
      std::cout << "~Visualization " << name_ << " (" << elements_.size() << " elems)" << std::endl;
   }
   // ----------------------------------------------------------------------
   std::string
   Visualization::
   name( void )
      const throw()
   { return name_; }
   // ----------------------------------------------------------------------
   std::string
   Visualization::
   description( void )
      const throw()
   { return name_; }
   // ----------------------------------------------------------------------
   void
   Visualization::
   set_world( const shawn::World& w )
      throw()
   {
      world_ = &w;
   }
   // ----------------------------------------------------------------------
   void
   Visualization::
   init( void )
      throw()
   {
      assert( world_ != NULL );
      add_element( elem_camera_ = init_camera() );
   }
   // ----------------------------------------------------------------------
   Camera*
   Visualization::
   init_camera( void )
      throw()
   {
      Camera* cam = new Camera;
      cam->init();
      return cam;
   }
   // ----------------------------------------------------------------------
   void
   Visualization::
   add_element( const ElementHandle& eh )
      throw( std::runtime_error )
   {
      if( elements_.find(eh->name()) != elements_.end() )
         throw std::runtime_error( std::string("duplicate visualization elements with name ") +
                                   eh->name() );
      elements_[eh->name()] = eh;
      
      Drawable* d = dynamic_cast<Drawable*>(eh.get());
      if( d != NULL )
         drawables_.push_back(d);

      eh->set_visualization(*this);
   }
   // ----------------------------------------------------------------------
class PriorityDescOrder
{
public:
   PriorityDescOrder( double t ):t_(t){};
   ~PriorityDescOrder(){};
   bool operator() ( const DrawableHandle& d1,
                     const DrawableHandle& d2 )
   { return d1->priority(t_) < d2->priority(t_); }
   double t_;
};
   void
   Visualization::
   draw( cairo_t* cr,
         double t,
         const Context& C )
      const throw( std::runtime_error )
   {
      PriorityDescOrder order(t);
      std::stable_sort(drawables_.begin(), drawables_.end(), order);
      for( DrawableList::const_iterator
              it    = drawables_.begin(),
              endit = drawables_.end();
           it != endit; ++it ) {
         try {
            (**it).draw(cr,t,C);
         }
         catch( std::runtime_error& re ) {
            std::ostringstream oss;
            oss << "cannot draw '" 
                << (**it).name()
                << "' at time " 
                << t
                << ": " << re.what();
            throw std::runtime_error(oss.str());
         }
      }
   }


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/base/visualization.cpp,v $
 * Version $Revision: 1.5 $
 * Date    $Date: 2006/02/14 21:53:10 $
 *-----------------------------------------------------------------------
 * $Log: visualization.cpp,v $
 * Revision 1.5  2006/02/14 21:53:10  ali
 * *** empty log message ***
 *
 * Revision 1.4  2006/02/04 20:19:46  ali
 * *** empty log message ***
 *
 * Revision 1.3  2006/02/01 17:07:29  ali
 * *** empty log message ***
 *
 * Revision 1.2  2006/01/31 12:44:41  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/01/29 21:02:01  ali
 * began vis
 *
 *-----------------------------------------------------------------------*/

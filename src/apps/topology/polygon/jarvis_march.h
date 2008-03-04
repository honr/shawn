#ifndef JARVIS_MARCH_H_
#define JARVIS_MARCH_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "point_2d.h"
#include "polygon.h"
#include <vector>

using namespace std;
using std::vector;

namespace polygon
{

   class JarvisMarch
   {
	   
   public:

	  JarvisMarch();      
      virtual ~JarvisMarch();

      virtual void set_name( const std::string& ) throw();
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
 
      virtual vector<Point2D> compute_convex_hull(vector<Point2D>*) throw();
      virtual void jm(void) throw();
      virtual int index_of_lowest_point(void) throw();
      virtual int index_of_rightmost_point_from(Point2D) throw();
      virtual void swap(int i, int j) throw();
	  
   private:
	  std::string name_;

   	  vector<Point2D> polygon_;
      int size_of_polygon_;			// size of polygon_
      int size_of_convex_hull_;		// number of nodes of the convex hull of a polygon
   };

}

#endif /*ENABLE_TOPOLOGY*/

#endif /*JARVIS_MARCH_H_*/

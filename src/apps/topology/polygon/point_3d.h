#ifndef POINT_3D_H_
#define POINT_3D_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include <string>

using namespace std;

namespace polygon
{

   class Point3D
   {
	   
   public:

	  Point3D();												// constructs a point at the coordinates (0,0,0)
	  Point3D(double, double, double);							// constructs a point at the specified coordinates (x,y,z)
      virtual ~Point3D();

      virtual void set_name( const std::string& ) throw();
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
   
	  virtual double get_x(void) throw();						// get the x coordinate of a point
	  virtual double get_y(void) throw();						// get the y coordinate of a point
	  virtual double get_z(void) throw();						// get the z coordinate of a point
	  virtual void set_location(double, double, double) throw();// set new x and y coordinates of a point 
	  virtual bool equals(Point3D) throw();						// returns true if two points are at the same position else false	  	  
	  
   private:
	  std::string name_;
	  
	  double x_;												// x coordinate
	  double y_;												// y coordinate
	  double z_;												// z coordinate
	  
	  // methods that have not been tested yet:
	  virtual Point3D relTo(Point3D p) throw();					// creates a new point relatively to point p as the origin
	  virtual bool isLess(Point3D p) throw();					// checks if the angle between the position vector of the new point and the origin
																// is smaller than the angle between the position vector of p and the origin
	  virtual Point3D cross(Point3D p) throw();					// cross product
	  virtual bool isFurther(Point3D p) throw();				// checks if the distance between the new point and the origin 
																// is shorter than the distance between p and the origin
	  virtual double mdist() throw();   						// Manhattan distance
	  	  
   };

}

#endif /*ENABLE_TOPOLOGY*/
#endif /*POINT_3D_H_*/

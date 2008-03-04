#ifndef BBOX_2D_H_
#define BBOX_2D_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "point_2d.h"
#include "point_3d.h"
#include <vector>

using namespace std;

namespace polygon
{

class Bbox2D
{
	   
public:

	Bbox2D();
	virtual ~Bbox2D();

	virtual void set_name( const std::string& ) throw();
	virtual std::string name( void ) const throw();
	virtual std::string description( void ) const throw();
	
	virtual Bbox2D getBoundingBox(vector<Point2D>*) throw();	// returns the bounding box of a polygon with 2-dimensional points
	
	virtual double get_min_x(void) throw();
	virtual double get_max_x(void) throw();
	virtual double get_min_y(void) throw();
	virtual double get_max_y(void) throw();
	virtual double get_width(void) throw();
	virtual double get_height(void) throw();
   
private:
 		
	std::string name_;	
	
	double min_x_;			// left border of the bounding box
	double max_x_;			// right border of the bounding box	
	double min_y_;			// bottom of the bounding box
	double max_y_;			// top of the bounding box
	double width_;			// width of the bounding box
	double height_;			// height of the bounding box	
	
	// methods that have not been tested yet:
	virtual Bbox2D getBoundingBox(vector<Point3D>*) throw();	// returns the bounding box of a polygon with 3-dimensional points
};

}

#endif
#endif /*BBOX_2D_H_*/

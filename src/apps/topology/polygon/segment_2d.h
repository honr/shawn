#ifndef SEGMENT_2D_H_
#define SEGMENT_2D_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "point_2d.h"
#include "bbox_2d.h"
#include "vector_2d.h"
#include "sys/vec.h"

using namespace std;
using namespace shawn;

namespace polygon
{

/** This class implements segments.
*
*	It provides functions for polygon topologies.
*
*/
class Segment2D
{
	   
public:

	Segment2D();
	Segment2D(Point2D, Point2D);
	Segment2D(Vec, Vec);
	virtual ~Segment2D();

	virtual void set_name( const std::string& ) throw();
	virtual std::string name( void ) const throw();
	virtual std::string description( void ) const throw();

	/** This method returns the source point of the segment.
	*
	*  \return Point2D : returns the source
	*/  	
	virtual Point2D get_source(void) throw();
	
	/** This method returns the sink point of the segment.
	*
	*  \return Point2D : returns the sink
	*/ 	
	virtual Point2D get_sink(void) throw();
	
	
	/** This method returns the smallest bounding box of the segment.
	*
	*  \return Bbox2D : returns the bounding box of the segment
	*/	
	virtual Bbox2D get_Bbox(void) throw();
	
	/** This method returns true if the point lies on the segment else returns false.
	*
	*  \param Point2D	: the point to check for whether it lies on the segment or not
	*  \return bool 	: true if the point lies on the segment else false
	*/		
	virtual bool point_on_segment(Point2D) throw();
	
	/** This method returns true if two segments have a point of intersection else returns false.
	* You have the chance to return the computed point of intersection as a reference.
	*
	*  \param Segment2D*: the second segment
	*  \param Point2D*	: return the point of intersection as a reference here
	*  \return bool 	: true if the two segments do have a point of intersection else false
	*/	
	virtual bool check_for_intersections(Segment2D*, Point2D*) throw();	
	
	/** This method returns true if two segments are equal else returns false.
	*
	*  \param Segment2D	: the second segment
	*  \return bool 	: true if the two segments are equal else false
	*/		
	virtual bool equals(Segment2D) throw();
   
private:
 		
	std::string name_;	
	
	Point2D source_;
	Point2D sink_;
	

};

}

#endif

#endif /*SEGMENT_2D_H_*/

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#define EPS 1e-16

#include "polygon.h"
#include <math.h>
#include <iostream>

using namespace std;
using std::vector;
using std::pair;

namespace polygon
{

	Polygon::
		Polygon()
	{
		
	}

	// ----------------------------------------------------------------------
	
	Polygon::
		Polygon(vector<Point2D> poly)
	{
		polygon_ = poly;
		segments_ = get_segments();	// compute the segments of the polygon
	}		
	
	// ----------------------------------------------------------------------	
	
	Polygon::
		~Polygon()
	{}

	// ----------------------------------------------------------------------
	void 
	Polygon::
		set_name(const std::string& s)
		throw()
	{ 
		name_ = s; 
	}		
	
	// ----------------------------------------------------------------------
	std::string
		Polygon::
		name(void)
		const throw()
	{
		return name_;
	}

	// ----------------------------------------------------------------------
	std::string
		Polygon::
		description(void)
		const throw()
	{ 
		return std::string("polygon"); 
	}

	// ----------------------------------------------------------------------	
	
	void 
		Polygon::
		push_back(Point2D p) 
		throw() 
	{
		bool contained = false;	// does polygon_ contain p_point yet?
				
		for(int i=0; i<((int)polygon_.size());i++){	
			Point2D p2=polygon_[i];
			if(p.equals(p2)){
				contained=true;
				break;
			}
		}
		if (!contained){		// add, if not contained yet
			polygon_.push_back(p);
			segments_ = get_segments();	// compute the segments of the polygon 
		}
	}
	
	// ----------------------------------------------------------------------
	  
	void 
		Polygon::
		delete_point(Point2D p1) 
		throw()
	{

		vector<Point2D> helping_polygon;
		for(int i=0; i<((int)polygon_.size());i++){			
			Point2D p2=polygon_[i];
			if(!(p1.equals(p2))){
				helping_polygon.push_back(p2);
			}
		}
		polygon_=helping_polygon;
		segments_ = get_segments();	// compute the segments of the polygon 
	}	
	
	// ----------------------------------------------------------------------	  
	
	int
		Polygon::
		get_numberofpoints(void) 
		throw()
	{
		return polygon_.size();
	}
	
	// ----------------------------------------------------------------------
	
	int 
		Polygon::
		contains_point(Point2D p1) 
		throw()
	{
		
		int index=0; 			// index of iterator 
		for(int i=0; i<((int)polygon_.size());i++){			
			Point2D p2=polygon_[i];
			if(p1.equals(p2)){
				return index;
			}
			index++;
		}
		return -1;
	}	
	
	// ----------------------------------------------------------------------
	
	int 
		Polygon::
		contains_point(Point2D p1, vector<Point2D> vec) 
		throw()
	{
		
		int index=0; 			// index of iterator 
		for(int i=0; i<((int)vec.size());i++){			
			Point2D p2=vec[i];
			if(p1.equals(p2)){
				return index;
			}
			index++;
		}
		return -1;
	} 
	
	// ----------------------------------------------------------------------
	
	vector<Point2D> 
		Polygon::
		get_poly_vector(void) 
		throw()
	{
		return polygon_;
	}		
	  
	// ----------------------------------------------------------------------	
	  
	vector<Segment2D> 
		Polygon::
		get_segments(void) 
		throw()
	{
		vector<Segment2D> segs;
		Point2D p1;
		Point2D p2;
		Segment2D edge;
		if(!is_empty()){
			for (vector<Point2D>::iterator it=polygon_.begin(); it!=polygon_.end(); it++){
				if(it==polygon_.begin()){
					p2=*it;
				}
				else {
					p1=p2;
					p2=*it;
					edge = Segment2D(p1,p2);
					segs.push_back(edge);
					
					
					if(it==(--polygon_.end())){
						edge = Segment2D(*it,*polygon_.begin());
						segs.push_back(edge);
					}
				}
			}
		}
		return segs;
	}
	
// ----------------------------------------------------------------------
	
	Bbox2D 
		Polygon::
		getBoundingBox(void) 
		throw() 
	{
		Bbox2D bbox;
		bbox = bbox.getBoundingBox(&polygon_);
		return bbox;
	}				
	
	// ----------------------------------------------------------------------
	
	bool 
		Polygon::
		bounded_side(Point2D p) // for simple polygons!
		throw()
	{
		
		// is p located inside or outside the polygon?
		Bbox2D bbox; 
		bbox.getBoundingBox(&polygon_);
		double bbox_min_x = bbox.get_min_x();
		double bbox_min_y = bbox.get_min_y();
		
		Point2D  p_not_in_bbox; // point outside the bounding box of the polygon.
		p_not_in_bbox.set_location(bbox_min_x-1,bbox_min_y-1);
		
		Segment2D seg1 = Segment2D(p_not_in_bbox, p);
		Segment2D seg2;
		
		vector<Point2D> intersection_points;
		for (vector<Segment2D>::iterator it1=segments_.begin(); it1!=segments_.end(); it1++){
			seg2 = *it1;
			Point2D ipoint;
			bool existing_intersection = seg1.check_for_intersections(&seg2, &ipoint); // compute the number of intersection points of seg1 with als segments of the polygon
			if(existing_intersection && (contains_point(ipoint, intersection_points)==-1)){
				intersection_points.push_back(ipoint);	
			}
		}
		
		if ((intersection_points.size() % 2) == 0){	// p is not positioned on bounded side		
			return false;
		}
		// p is positioned on bounded side
		return true;
	}

	// ----------------------------------------------------------------------
	
	bool 
		Polygon::
		on_boundary(Point2D p) // for simple polygons!
		throw()
	{
		// is p located on any segment of the polygon?
		Segment2D seg;
		for (vector<Segment2D>::iterator it1=segments_.begin(); it1!=segments_.end(); it1++){
			seg = *it1;
			if(seg.point_on_segment(p)){
				return true;
			}
		}
		return false;		
	}
	
	// ----------------------------------------------------------------------
	
	bool 
		Polygon::	
		is_simple() 
		throw()
	{
		Segment2D seg1;
		Segment2D seg2;
		Point2D ipoint;
		
		for (vector<Segment2D>::iterator it1=segments_.begin(); it1!=segments_.end(); it1++){
			seg1 = *it1;
			for (vector<Segment2D>::iterator it2=++it1; it2!=segments_.end(); it2++){
				seg2 = *it2;
				
				bool intersection = seg1.check_for_intersections(&seg2, &ipoint);
				if (intersection){	
					return false;
				}
				
			}
			--it1;
		}
		
		return true;
	}
	
	// ----------------------------------------------------------------------
	
	bool 
		Polygon::
		is_empty() 
		throw()
	{
		if(polygon_.size()==0){
			return true;
		}
		return false;
	}	

	// ----------------------------------------------------------------------	
	
	Polygon 
		Polygon::
		compute_convex_hull() 
		throw()
	{
		JarvisMarch jm;
		vector<Point2D> convex_hull = jm.compute_convex_hull(&polygon_);
		Polygon poly_convex = Polygon(convex_hull);
		return poly_convex;
	}

	  // ----------------------------------------------------------------------
	
	std::ostream&
	    operator << ( std::ostream& os, const Polygon& p )
	    throw()
	{

		for (Polygon::const_iterator it =p.begin(); it!= p.end();it++){
			os << "(" << it->get_x() << ", " << it->get_y() << ")";
		}
		return os;
	}

	
}

#endif

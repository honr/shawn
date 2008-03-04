#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#define EPS 1e-3

using namespace std;

#include "segment_2d.h"
#include <math.h>
#include <iostream>

namespace polygon
{

	Segment2D::
		Segment2D()
	{}

	// ----------------------------------------------------------------------	
	
	Segment2D::
		Segment2D(Point2D p1, Point2D p2)
	{
		source_ = p1;
		sink_ = p2;
	} 

	// ----------------------------------------------------------------------	
	
	Segment2D::
		Segment2D(Vec v1, Vec v2)
	{
		Point2D p1;
		p1.set_location(v1.x(), v1.y());
		Point2D p2;
		p2.set_location(v2.x(), v2.y());
		source_ = p1;
		sink_ = p2;
	} 	
	
	// ----------------------------------------------------------------------	
	
	Segment2D::
		~Segment2D()
	{}

	// ----------------------------------------------------------------------
	void 
		Segment2D::
		set_name(const std::string& s)
		throw()
	{ 
		name_ = s; 
	}		
	
	// ----------------------------------------------------------------------
	std::string
		Segment2D::
		name(void)
		const throw()
	{
		return name_;
	}

	// ----------------------------------------------------------------------
	std::string
		Segment2D::
		description(void)
		const throw()
	{ 
		return std::string("an edge between two vertices. In contrast to a line a segment really ends at its source and sink!"); 
	}
	
	// ----------------------------------------------------------------------	

	Point2D 
		Segment2D::
		get_source(void) 
		throw()
	{
		return source_;
	}

	// ----------------------------------------------------------------------	
	

	Point2D 
		Segment2D::
		get_sink(void) 
		throw()
	{
		return sink_;
	}

	// ----------------------------------------------------------------------	
	

	Bbox2D 
		Segment2D::
		get_Bbox(void) 
		throw()
	{
		Bbox2D box;
		vector<Point2D>  seg;
		seg.push_back(source_);
		seg.push_back(sink_);
		box.getBoundingBox(&seg);
		return box;
	}	

	// ----------------------------------------------------------------------		
	
	bool
		Segment2D::
		point_on_segment(Point2D p) 
		throw()
	{	
		Bbox2D bbox; 
		vector<Point2D> segment;
		segment.push_back(source_);
		segment.push_back(sink_);
		bbox.getBoundingBox(&segment);
		double bbox_min_x = bbox.get_min_x();
		double bbox_max_x = bbox.get_max_x();
		double bbox_min_y = bbox.get_min_y();
		double bbox_max_y = bbox.get_max_y();
		
		double p_x = p.get_x();
		double p_y = p.get_y();
		if ( ((bbox_min_x-p_x)<= EPS) && ((bbox_max_x-p_x)>= -EPS) && ((bbox_min_y-p_y)<= EPS) && ((bbox_max_y-p_y)>= -EPS) ){
			
			//compute normal vector of seg
			Vector2D p_vec = Vector2D(p);
			Vector2D source_vec = Vector2D(source_);
			Vector2D sink_vec = Vector2D(sink_);
			Vector2D dv = sink_vec - source_vec;// direction vector
			Vector2D n; // normal vector
			
			if(fabs(dv.get_x())<EPS){ // dv is a vertical vector
				double dv_x = dv.get_x();
				double dv_y = dv.get_y();
				n.set_x(dv_y);
				n.set_y(dv_x);
			}
			else{				
				n.set_x(- (dv.get_y() / dv.get_x()));
				n.set_y(1);
			}
			double length_of_n = n.get_length();
			Vector2D p_minus_a = p_vec - source_vec; // a = point on seg (=source of seg)
			double help = fabs(n * p_minus_a);
			double distance_point_segment = help / length_of_n; // |n*(P-A)| / |n|
			
			if (fabs(distance_point_segment)<EPS){
				return true;
			}
		}
		return false;		
	}
	
	// ----------------------------------------------------------------------	
	
	bool 
		Segment2D::
		check_for_intersections(Segment2D* p_seg, Point2D* ipoint) 
		throw()	
	{
		if (equals(*p_seg)) {
			return true;
		}
		
		// compute point of intersection:
		
		//xA = x1 + lambda*(x2-x1)
		//yA = y1 + lambda*(y2-y1)
		double x1 = source_.get_x(); 
		double x2 = sink_.get_x();
		double y1 = source_.get_y();
		double y2 =	sink_.get_y();
			
		//xB = x3 + mue*(x4-x3)
		//yB = y3 + mue*(y4-y3)	
		double x3 = (*p_seg).get_source().get_x();
		double x4 = (*p_seg).get_sink().get_x();
		double y3 = (*p_seg).get_source().get_y();
		double y4 =	(*p_seg).get_sink().get_y();			
	
		double mue = (x1*y2 + x2*y3 + x3*y1 - x2*y1 - x3*y2 - x1*y3) /
					 (x1*y4 + x4*y2 + x2*y3 + x3*y1 - x4*y1 - x1*y3 -x3*y2 - x2*y4);
		double lambda =	(x3 + mue*x4 - mue*x3 -x1) / (x2 - x1);
		
		// s1: point of intersection located on this segment (owner)
		double x_s1 = x1 + lambda*(x2-x1);
		double y_s1 = y1 + lambda*(y2-y1);
		
		// s2: point of intersection located on p_seg
		double x_s2 = x3 + mue*(x4-x3);
		double y_s2 = y3 + mue*(y4-y3);
				
		//----------------------------------------------------------
		
		// (s1==s2?) 
		if( ((fabs(x_s1 - x_s2) < EPS)) && ((fabs(y_s1 - y_s2) < EPS))){
			//the point of intersection can not be an end point of a segment!
			if( !((((fabs(x_s1 - source_.get_x()) < EPS)) && ((fabs(y_s1 - source_.get_y()) <EPS))) || (((fabs(x_s1 - sink_.get_x()) < EPS)) && ((fabs(y_s1 - sink_.get_y()) <EPS)))) ){ 			
				// does the point of intersection s lie on both segments? (segments are no lines!)
				Point2D s;
				s.set_location(x_s1, y_s1);	
			
				if(point_on_segment(s) && (*p_seg).point_on_segment(s)){
					*ipoint=s;
					return true;
				}
			}
		}
		
		return false;	// returns false if there does not exist any point of intersection between seg1 and seg2
		
	}

	// ----------------------------------------------------------------------
	
	bool 
		Segment2D::
		equals(Segment2D s) 
		throw()	
	{
		if(	(source_.equals(s.get_source()) && sink_.equals(s.get_sink())) || 
			(source_.equals(s.get_sink())   && sink_.equals(s.get_source())) )
		{
			return true;
		}
		
		return false;
	}
	
	
}

#endif	

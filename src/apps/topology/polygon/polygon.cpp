#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY


#include "apps/topology/polygon/polygon.h"
#include <math.h>
#include <iostream>

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
		Polygon(const vector<Vec>& poly)
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
		push_back(const Vec& p) 
		throw(std::runtime_error) 
	{
		bool contained = false;	// does polygon_ contain p_point yet?
				
		for(int i=0; i<((int)polygon_.size());i++)
		{	
			Vec p2=polygon_[i];
			if(p==p2)
			{
				contained=true;
				throw runtime_error("In a simple polygon a point is contained only once. The point you want to add to the polygon is already contained yet.");
			}
		}
		if (!contained)
		{		// add, if not contained yet
			polygon_.push_back(p);
			segments_ = get_segments();	// compute the segments of the polygon 
		}
	}
	
	// ----------------------------------------------------------------------
	  
	void 
		Polygon::
		delete_point(const Vec& p1) 
		throw()
	{
		iterator it = find(p1);
		if( it != polygon_.end() )
		{
			polygon_.erase(it);
		}
		segments_ = get_segments();	// compute the segments of the polygon 	
	}	
	
	// ----------------------------------------------------------------------	  
	
	int
		Polygon::
		get_numberofpoints(void) const 
		throw()
	{
		return polygon_.size();
	}
	
	// ----------------------------------------------------------------------
	
	bool 
		Polygon::
		contains_point(const Vec& p1) const 
		throw()
	{
		
		return std::find(polygon_.begin(), polygon_.end(), p1) != polygon_.end();
	}	
	
	// ----------------------------------------------------------------------
	
	bool 
		Polygon::
		contains_point(const Vec& p1, const vector<Vec>& vec) const 
		throw()
	{
		
		return std::find(vec.begin(), vec.end(), p1) != vec.end();
		
	} 
	
	// ----------------------------------------------------------------------
	
	vector<Vec> 
		Polygon::
		get_poly_vector(void) const 
		throw()
	{
		return polygon_;
	}		
	  
	// ----------------------------------------------------------------------	
	  
	vector<Segment2D> 
		Polygon::
		get_segments(void) const 
		throw()
	{
		vector<Segment2D> segs;
		Vec p1;
		Vec p2;
		Segment2D edge;
		if(!is_empty())
		{
			for (vector<Vec>::const_iterator it=polygon_.begin(); it!=polygon_.end(); it++)
			{
				if(it==polygon_.begin())
				{
					p2=*it;
				}
				else {
					p1=p2;
					p2=*it;
					edge = Segment2D(p1,p2);
					segs.push_back(edge);
					
					
					if(it==(--polygon_.end()))
					{
						edge = Segment2D(*it,*polygon_.begin());
						segs.push_back(edge);
					}
				}
			}
		}
		return segs;
	}
	
// ----------------------------------------------------------------------
	
	Box 
		Polygon::
		getBoundingBox(void) const 
		throw() 
	{
		double x;
		double y;
		
		double min_x;
		double max_x;
		double min_y;
		double max_y;
		
		for(vector<Vec>::const_iterator it=polygon_.begin(); it!=polygon_.end(); it++)
		{
			Vec p = *it;
			x = p.x();
			y = p.y();
			
			if(it==polygon_.begin())
			{
				min_x = x;
				max_x = x;
				min_y = y;
				max_y = y;
			}
			else
			{
				if(x<min_x)
				{
					min_x = x;
				}
				if(x>max_x)
				{
					max_x = x;
				}
				if(y<min_y)
				{
					min_y = y;
				}
				if(y>max_y)
				{
					max_y = y;
				}
			}
			
		}
		
		Vec lower = Vec(min_x, min_y, 0.0);
		Vec upper = Vec(max_x, max_y, 0.0);
		
		return Box(lower, upper);
	}				
	
	// ----------------------------------------------------------------------
	
	bool 
		Polygon::
		bounded_side(const Vec& p) const // for simple polygons!
		throw()
	{
		
		// is p located inside or outside the polygon?
		Box bbox = getBoundingBox();
		double bbox_min_x = bbox.lower().x();
		double bbox_min_y = bbox.lower().y();
		
		Vec  p_not_in_bbox = Vec(bbox_min_x-1,bbox_min_y-1, 0.0); // point outside the bounding box of the polygon.
		
		Segment2D seg1 = Segment2D(p_not_in_bbox, p);
		Segment2D seg2;
		
		vector<Vec> intersection_points;
		for (vector<Segment2D>::const_iterator it1=segments_.begin(); it1!=segments_.end(); it1++)
		{
			seg2 = *it1;
			Vec ipoint;
			bool existing_intersection = seg1.check_for_intersections(seg2, ipoint); // compute the number of intersection points of seg1 with als segments of the polygon
			if(existing_intersection && (!contains_point(ipoint, intersection_points)))
			{
				intersection_points.push_back(ipoint);	
			}
		}
		if ((intersection_points.size() % 2) == 0)
		{	// p is not positioned on bounded side		
			return false;
		}
		// p is positioned on bounded side
		return true;
	}

	// ----------------------------------------------------------------------
	
	bool 
		Polygon::
		on_boundary(const Vec& p) const // for simple polygons!
		throw()
	{
		// is p located on any segment of the polygon?
		Segment2D seg;
		for (vector<Segment2D>::const_iterator it1=segments_.begin(); it1!=segments_.end(); it1++)
		{
			seg = *it1;
			if(seg.point_on_segment(p))
			{
				return true;
			}
		}
		return false;		
	}
	
	// ----------------------------------------------------------------------
	
	bool 
		Polygon::	
		is_simple() const 
		throw()
	{
		Segment2D seg1;
		Segment2D seg2;
		Vec ipoint;
		
		for (vector<Segment2D>::const_iterator it1=segments_.begin(); it1!=segments_.end(); it1++)
		{
			seg1 = *it1;
			for (vector<Segment2D>::const_iterator it2=++it1; it2!=segments_.end(); it2++)
			{
				seg2 = *it2;
				
				bool intersection = seg1.check_for_intersections(seg2, ipoint);
				if (intersection)
				{	
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
		is_empty() const 
		throw()
	{
		if(polygon_.size()==0)
		{
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
		vector<Vec> convex_hull = jm.compute_convex_hull(polygon_);
		Polygon poly_convex = Polygon(convex_hull);
		return poly_convex;
	}

	  // ----------------------------------------------------------------------
	
	std::ostream&
	    operator << ( std::ostream& os, const Polygon& p )
	    throw()
	{

		for (Polygon::const_iterator it =p.begin(); it!= p.end();it++)
		{
			os << "(" << it->x() << ", " << it->y() << ")";
		}
		return os;
	}

	
}

#endif

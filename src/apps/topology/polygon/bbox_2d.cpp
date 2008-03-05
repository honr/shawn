#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/polygon/bbox_2d.h"

using namespace std;

namespace polygon
{

	Bbox2D::
		Bbox2D()
	{}


	// ----------------------------------------------------------------------	
	
	Bbox2D::
		~Bbox2D()
	{}

	// ----------------------------------------------------------------------	
	
	Bbox2D
		Bbox2D::
		getBoundingBox(const vector<Vec>& polygon) 
		throw()
	{	
	
		double x;
		double y;
		for(vector<Vec>::const_iterator it=polygon.begin(); it!=polygon.end(); it++)
		{
			Vec p = *it;
			x = p.x();
			y = p.y();
				
			if(it==polygon.begin())
			{
				min_x_ = x;
				max_x_ = x;
				min_y_ = y;
				max_y_ = y;
			}
			else{
				if(x<min_x_)
				{	//compute min_x
					min_x_ = x;
				}
				if(x>max_x_)
				{	//compute max_x
					max_x_ = x;
				}
				if(y<min_y_)
				{	//compute min_y
					min_y_ = y;
				}
				if(y>max_y_)
				{	//compute max_y
					max_y_ = y;
				}
			}			
		}
		width_  = max_x_ - min_x_;	// compute the width of the bounding box
		height_ = max_y_ - min_y_;	// compute the height of the bounding box
		return *this;
	}	
	
	// ----------------------------------------------------------------------	
	
	double 
		Bbox2D::
		get_min_x(void) const
		throw()
	{
		return min_x_;
	}
	
	// ----------------------------------------------------------------------	
	
	double
		Bbox2D::
		get_max_x(void) const
		throw()
	{
		return max_x_;
	}		
		
	// ----------------------------------------------------------------------	
	
	double 
		Bbox2D::
		get_min_y(void) const
		throw()
	{
		return min_y_;
	}
	
	// ----------------------------------------------------------------------	
	
	double
		Bbox2D::
		get_max_y(void) const
		throw()
	{
		return max_y_;
	}
		
	// ----------------------------------------------------------------------	
	
	double
		Bbox2D::
		get_width(void) const
		throw()
	{
		return width_;
	}
		
	// ----------------------------------------------------------------------	
	
	double
		Bbox2D::
		get_height(void) const
		throw()	
	{
		return height_;
	}
}

#endif	

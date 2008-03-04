#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "bbox_2d.h"

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
	void 
		Bbox2D::
		set_name(const std::string& s)
		throw()
	{ 
		name_ = s; 
	}		
	
	// ----------------------------------------------------------------------
	std::string
		Bbox2D::
		name(void)
		const throw()
	{
		return name_;
	}

	// ----------------------------------------------------------------------
	std::string
		Bbox2D::
		description(void)
		const throw()
	{ 
		return std::string("a 2-dimensional bounding box"); 
	}
	
	// ----------------------------------------------------------------------	
	
	Bbox2D
		Bbox2D::
		getBoundingBox(vector<Point2D>* polygon) 
		throw()
	{	
	
		double x;
		double y;
		for(vector<Point2D>::iterator it=(*polygon).begin(); it!=(*polygon).end(); it++)
		{
			Point2D p = *it;
			x = p.get_x();
			y = p.get_y();
				
			if(it==(*polygon).begin()){
				min_x_ = x;
				max_x_ = x;
				min_y_ = y;
				max_y_ = y;
			}
			else{
				if(x<min_x_){	//compute min_x
					min_x_ = x;
				}
				if(x>max_x_){	//compute max_x
					max_x_ = x;
				}
				if(y<min_y_){	//compute min_y
					min_y_ = y;
				}
				if(y>max_y_){	//compute max_y
					max_y_ = y;
				}
			}			
		}
		width_  = max_x_ - min_x_;	// compute the width of the bounding box
		height_ = max_y_ - min_y_;	// compute the height of the bounding box
		return *this;
	}	
	
	// ----------------------------------------------------------------------	
	
	Bbox2D
		Bbox2D::
		getBoundingBox(vector<Point3D>* polygon) 
		throw()
	{	
		double x;
		double y;
		for(vector<Point3D>::iterator it=(*polygon).begin(); it!=(*polygon).end(); it++)
		{
			Point3D p = *it;
			x = p.get_x();
			y = p.get_y();
				
			if(it==(*polygon).begin()){
				min_x_ = x;
				max_x_ = x;
				min_y_ = y;
				max_y_ = y;
			}
			else{
				if(x<min_x_){	//compute min_x
					min_x_ = x;
				}
				if(x>max_x_){	//compute max_x
					max_x_ = x;
				}
				if(y<min_y_){	//compute min_y
					min_y_ = y;
				}
				if(y>max_y_){	//compute max_y
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
		get_min_x(void) 
		throw()
	{
		return min_x_;
	}
	
	// ----------------------------------------------------------------------	
	
	double
		Bbox2D::
		get_max_x(void) 
		throw()
	{
		return max_x_;
	}		
		
	// ----------------------------------------------------------------------	
	
	double 
		Bbox2D::
		get_min_y(void) 
		throw()
	{
		return min_y_;
	}
	
	// ----------------------------------------------------------------------	
	
	double
		Bbox2D::
		get_max_y(void) 
		throw()
	{
		return max_y_;
	}
		
	// ----------------------------------------------------------------------	
	
	double
		Bbox2D::
		get_width(void) 
		throw()
	{
		return width_;
	}
		
	// ----------------------------------------------------------------------	
	
	double
		Bbox2D::
		get_height(void) 
		throw()	
	{
		return height_;
	}
}

#endif	

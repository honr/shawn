#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#define EPS 1e-3

#include "point_2d.h"
#include <math.h>

using namespace std;


namespace polygon
{

		Point2D::
		Point2D()
		: name_(""),
		x_(0),
		y_(0)
	{
	}
	
	// ----------------------------------------------------------------------
		
		Point2D::
		Point2D(double p_x, double p_y)
	{
		x_ = p_x;
		y_ = p_y;
	}

	// ----------------------------------------------------------------------		
		Point2D::
		~Point2D()
	{
			
	}

	// ----------------------------------------------------------------------
	void 
		Point2D::
		set_name(const std::string& s)
		throw()
	{ 
		name_ = s; 
	}		
	
	// ----------------------------------------------------------------------
	std::string
		Point2D::
		name(void)
		const throw()
	{
		return name_;
	}

	// ----------------------------------------------------------------------
	std::string
		Point2D::
		description(void)
		const throw()
	{ 
		return std::string("2-dimensional point"); 
	}

	// ----------------------------------------------------------------------	
	
	double 
		Point2D::
		get_x(void) const
		throw()
	{
		return x_;
	}	
	  
	// ----------------------------------------------------------------------
	  
	double 
		Point2D::
		get_y(void) const 
		throw()
	{
		return y_;
	}

	// ----------------------------------------------------------------------
	  
	void 
		Point2D::
		set_location(double p_x, double p_y) 
		throw()
	{
		x_ = p_x;
		y_ = p_y;
	}
	
	// ----------------------------------------------------------------------	
	
	bool 
		Point2D::
		equals(Point2D p) 
		throw()
	{
		if((fabs(x_-p.get_x())<EPS)&&(fabs(y_-p.get_y())<EPS)){
			return true;
		}
		else return false;
		
	}
	
	// ----------------------------------------------------------------------	
	
	Point2D 
		Point2D::
		relTo(Point2D p) 
		throw()
	{
		return Point2D(x_-p.get_x(), y_-p.get_y());
	}

	// ----------------------------------------------------------------------

	bool 
		Point2D::
		isLess(Point2D p) 
		throw()
	{
		double f=cross(p);
		return f>0 || (fabs(f-0)<EPS) && isFurther(p);
	}

	// ----------------------------------------------------------------------

	double 
		Point2D::
		cross(Point2D p) 
		throw()
	{
		return x_*p.get_y()-p.get_x()*y_;
	}

	// ----------------------------------------------------------------------

	bool 
		Point2D::	
		isFurther(Point2D p) 
		throw()
	{
		return mdist()>p.mdist();
	}

	// ----------------------------------------------------------------------

	double 
		Point2D::
		mdist() 
		throw()   
	{
		return fabs(x_)+fabs(y_);
	}
		
}

#endif

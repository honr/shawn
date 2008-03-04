#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#define EPS 1e-3

#include "point_3d.h"
#include <math.h>

using namespace std;


namespace polygon
{

		Point3D::
		Point3D()
	{
		x_ = 0;	
		y_ = 0;	
		z_ = 0;
	}

	// ----------------------------------------------------------------------
		
		Point3D::
		Point3D(double p_x, double p_y, double p_z)
	{
		x_ = p_x;
		y_ = p_y;
		z_ = p_z;
	}

	// ----------------------------------------------------------------------		
		Point3D::
		~Point3D()
	{
			
	}

	// ----------------------------------------------------------------------
	void 
		Point3D::
		set_name(const std::string& s)
		throw()
	{ 
		name_ = s; 
	}		
	
	// ----------------------------------------------------------------------
	std::string
		Point3D::
		name(void)
		const throw()
	{
		return name_;
	}

	// ----------------------------------------------------------------------
	std::string
		Point3D::
		description(void)
		const throw()
	{ 
		return std::string("3-dimensional point"); 
	}

	// ----------------------------------------------------------------------	
	
	double 
		Point3D::
		get_x(void) 
		throw()
	{
		return x_;
	}	
	  
	// ----------------------------------------------------------------------
	  
	double 
		Point3D::
		get_y(void) 
		throw()
	{
		return y_;
	}
	
	// ----------------------------------------------------------------------
	  
	double 
		Point3D::
		get_z(void) 
		throw()
	{
		return z_;
	}
	
	// ----------------------------------------------------------------------
	  
	void 
		Point3D::
		set_location(double p_x, double p_y, double p_z) 
		throw()
	{
		x_ = p_x;
		y_ = p_y;
		z_ = p_z;
	}
	
	// ----------------------------------------------------------------------	
	
	bool 
		Point3D::
		equals(Point3D p) 
		throw()
	{
		if((fabs(x_-p.get_x())<EPS)&&(fabs(y_-p.get_y())<EPS)&&(fabs(z_-p.get_z())<EPS)){
			return true;
		}
		else return false;
		
	}

	// ----------------------------------------------------------------------	
	
	Point3D 
		Point3D::
		relTo(Point3D p) 
		throw()
	{
		return Point3D(x_-p.get_x(), y_-p.get_y(), z_-p.get_z());
	}

	// ----------------------------------------------------------------------

	bool 
		Point3D::
		isLess(Point3D p) 
		throw()
	{
		Point3D cr = cross(p);
		double f=cr.get_z();
		return f>0 || (fabs(f-0)<EPS) && isFurther(p);
	}

	// ----------------------------------------------------------------------

	Point3D 
		Point3D::
		cross(Point3D p) 
		throw()
	{
		Point3D cr;
		double cr_x = y_*p.get_z()-p.get_y()*z_;
		double cr_y = z_*p.get_x()-p.get_z()*x_;
		double cr_z = x_*p.get_y()-p.get_x()*y_;
		cr.set_location(cr_x, cr_y, cr_z);
		return cr;
	}

	// ----------------------------------------------------------------------

	bool 
		Point3D::	
		isFurther(Point3D p) 
		throw()
	{
		return mdist()>p.mdist();
	}

	// ----------------------------------------------------------------------

	double 
		Point3D::
		mdist() 
		throw()   // Manhattan-Distanz
	{
		return fabs(x_)+fabs(y_)+fabs(z_);
	}
	
}

#endif

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

using namespace std;

#include "vector_2d.h"

namespace polygon
{

	Vector2D::
		Vector2D()
	{}

	// ----------------------------------------------------------------------	

	Vector2D::
		Vector2D(Point2D p)
	{
		x_ = p.get_x();
		y_ = p.get_y();
	}

	// ----------------------------------------------------------------------	
	Vector2D::
		~Vector2D()
	{}

	// ----------------------------------------------------------------------
	void 
		Vector2D::
		set_name(const std::string& s)
		throw()
	{ 
		name_ = s; 
	}		
	
	// ----------------------------------------------------------------------
	std::string
		Vector2D::
		name(void)
		const throw()
	{
		return name_;
	}

	// ----------------------------------------------------------------------
	std::string
		Vector2D::
		description(void)
		const throw()
	{ 
		return std::string("a 2-dimensional vector"); 
	}
	
	// ----------------------------------------------------------------------		
	
	double 
		Vector2D::
		get_x(void) 
		throw()
	{
		return x_;
	}
	
	// ----------------------------------------------------------------------	
	
	double
		Vector2D::
		get_y(void) 
		throw()
	{
		return y_;
	}		

	// ----------------------------------------------------------------------		
	
	void 
		Vector2D::
		set_x(double p) 
		throw()
	{
		x_ = p;
	}
	
	// ----------------------------------------------------------------------	
	
	void
		Vector2D::
		set_y(double p) 
		throw()
	{
		y_ = p;
	}		
	
	// ----------------------------------------------------------------------	

	Vector2D 
		Vector2D::
		operator+(Vector2D v) 
		throw()	
	{
		Vector2D result;
		result.set_x(x_ + v.get_x());
		result.set_y(y_ + v.get_y());
		return result;
	}

	// ----------------------------------------------------------------------		
	
	double 
		Vector2D::
		get_length(void) 
		throw()
	{
		double length = (x_ * x_) + (y_ * y_);
		return length;
	}
	
	// ----------------------------------------------------------------------	
	
	Vector2D 
		Vector2D::
		operator-(Vector2D v) 
		throw()
	{
		Vector2D result;
		result.set_x(x_ - v.get_x());
		result.set_y(y_ - v.get_y());
		return result;		
	}
	
	// ----------------------------------------------------------------------	
	
	double 
		Vector2D::
		operator*(Vector2D v) 
		throw()
	{
		double result;
		result = (x_ * v.get_x()) + (y_ * v.get_y());
		return result;		
	}
	
	// ----------------------------------------------------------------------	
	
	double 
		Vector2D::
		operator/(Vector2D v) 
		throw()
	{
		Vector2D inverse_v;
		inverse_v.set_x(1/v.get_x());
		inverse_v.set_y(1/v.get_y());
		
		double result = *this * inverse_v;
		return result;
	}
	
	// ----------------------------------------------------------------------	
	
	Vector2D 
		Vector2D::
		operator*(double s) 
		throw()
	{
		Vector2D result;
		result.set_x(x_ * s);
		result.set_y(y_ * s);
		return result;	
	}
	
	// ----------------------------------------------------------------------	
	
	Vector2D 
		Vector2D::
		operator/(double s) 
		throw()	
	{
		Vector2D result;
		result.set_x(x_ / s);
		result.set_y(y_ / s);
		return result;	
	}
			

}

#endif	

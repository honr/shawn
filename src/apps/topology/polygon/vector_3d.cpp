#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

using namespace std;

#include "vector_3d.h"

namespace polygon
{

	Vector3D::
		Vector3D()
	{}


	// ----------------------------------------------------------------------	
	
	Vector3D::
		~Vector3D()
	{}

	// ----------------------------------------------------------------------
	void 
		Vector3D::
		set_name(const std::string& s)
		throw()
	{ 
		name_ = s; 
	}		
	
	// ----------------------------------------------------------------------
	std::string
		Vector3D::
		name(void)
		const throw()
	{
		return name_;
	}

	// ----------------------------------------------------------------------
	std::string
		Vector3D::
		description(void)
		const throw()
	{ 
		return std::string("a 3-dimensional vector"); 
	}
	
	// ----------------------------------------------------------------------	
	
	
	double 
		Vector3D::
		get_x(void) 
		throw()
	{
		return x_;
	}
	
	// ----------------------------------------------------------------------	
	
	double
		Vector3D::
		get_y(void) 
		throw()
	{
		return y_;
	}		

	// ----------------------------------------------------------------------	
	
	double
		Vector3D::
		get_z(void) 
		throw()
	{
		return z_;
	}		
	
	// ----------------------------------------------------------------------		
	
	void 
		Vector3D::
		set_x(double p) 
		throw()
	{
		x_ = p;
	}
	
	// ----------------------------------------------------------------------	
	
	void
		Vector3D::
		set_y(double p) 
		throw()
	{
		y_ = p;
	}		
	
	// ----------------------------------------------------------------------	
	
	void
		Vector3D::
		set_z(double p) 
		throw()
	{
		z_ = p;
	}	
	
	// ----------------------------------------------------------------------	

	Vector3D 
		Vector3D::
		operator+(Vector3D v) 
		throw()	
	{
		Vector3D result;
		result.set_x(x_ + v.get_x());
		result.set_y(y_ + v.get_y());
		result.set_z(z_ + v.get_z());
		return result;
	}
	
	// ----------------------------------------------------------------------	
	
	Vector3D 
		Vector3D::
		operator-(Vector3D v) 
		throw()
	{
		Vector3D result;
		result.set_x(x_ - v.get_x());
		result.set_y(y_ - v.get_y());
		result.set_z(z_ - v.get_z());
		return result;		
	}
	
	// ----------------------------------------------------------------------	
	
	double 
		Vector3D::
		operator*(Vector3D v) 
		throw()
	{
		double result;
		result = (x_ * v.get_x()) + (y_ * v.get_y()) + (z_ * v.get_z());
		return result;		
	}
	
	// ----------------------------------------------------------------------	
	
	double
		Vector3D::
		operator/(Vector3D v) 
		throw()
	{
		Vector3D inverse_v;
		inverse_v.set_x(1/v.get_x());
		inverse_v.set_y(1/v.get_y());
		inverse_v.set_z(1/v.get_z());
		
		double result = *this * inverse_v;
		return result;
	}
	
	// ----------------------------------------------------------------------	
	
	Vector3D 
		Vector3D::
		operator*(double s) 
		throw()
	{
		Vector3D result;
		result.set_x(x_ * s);
		result.set_y(y_ * s);
		result.set_z(z_ * s);
		return result;	
	}
	
	// ----------------------------------------------------------------------	
	
	Vector3D 
		Vector3D::
		operator/(double s) 
		throw()	
	{
		Vector3D result;
		result.set_x(x_ / s);
		result.set_y(y_ / s);
		result.set_z(z_ / s);
		return result;	
	}		

}

#endif	

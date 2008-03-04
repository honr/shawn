#ifndef VECTOR_3D_H_
#define VECTOR_3D_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "point_3d.h"
#include <string>

using namespace std;

namespace polygon
{

class Vector3D
{
	   
public:

	Vector3D();
	virtual ~Vector3D();

	virtual void set_name( const std::string& ) throw();
	virtual std::string name( void ) const throw();
	virtual std::string description( void ) const throw();
		
	virtual double get_x(void) throw();
	virtual double get_y(void) throw();
	virtual double get_z(void) throw();
	virtual void set_x(double) throw();
	virtual void set_y(double) throw();	
	virtual void set_z(double) throw();
	
	virtual Vector3D operator+(Vector3D) throw();	
	virtual Vector3D operator-(Vector3D) throw();
	virtual double operator*(Vector3D) throw();
	virtual double operator/(Vector3D) throw();
	virtual Vector3D operator*(double) throw();
	virtual Vector3D operator/(double) throw();	
	
private:
 		
	std::string name_;	
	
	double x_;			
	double y_;
	double z_;
};

}

#endif

#endif /*VECTOR_3D_H_*/

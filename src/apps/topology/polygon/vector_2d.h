#ifndef VECTOR_2D_H_
#define VECTOR_2D_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "point_2d.h"
#include <string>

using namespace std;

namespace polygon
{

class Vector2D
{
	   
public:

	Vector2D();
	Vector2D(Point2D);
	virtual ~Vector2D();

	virtual void set_name( const std::string& ) throw();
	virtual std::string name( void ) const throw();
	virtual std::string description( void ) const throw();
	
	virtual double get_x(void) throw();
	virtual double get_y(void) throw();
	virtual void set_x(double) throw();
	virtual void set_y(double) throw();
	virtual double get_length(void) throw();
	
	virtual Vector2D operator+(Vector2D) throw();	
	virtual Vector2D operator-(Vector2D) throw();
	virtual double operator*(Vector2D) throw();
	virtual double operator/(Vector2D) throw();
	virtual Vector2D operator*(double) throw();
	virtual Vector2D operator/(double) throw();
   
private:
 		
	std::string name_;	
	
	double x_;			
	double y_;
};

}

#endif

#endif /*VECTOR_2D_H_*/

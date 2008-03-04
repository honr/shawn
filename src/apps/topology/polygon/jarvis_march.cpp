#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#define EPS 1e-3

#include "jarvis_march.h"
#include <math.h>

using namespace std;

namespace polygon
{

	JarvisMarch::
	JarvisMarch()
	{
		
	}

	// ----------------------------------------------------------------------	
	
	JarvisMarch::
		~JarvisMarch()
	{}

	// ----------------------------------------------------------------------
	void 
		JarvisMarch::
		set_name(const std::string& s)
		throw()
	{ 
		name_ = s; 
	}		
	
	// ----------------------------------------------------------------------
	std::string
		JarvisMarch::
		name(void)
		const throw()
	{
		return name_;
	}

	// ----------------------------------------------------------------------
	std::string
		JarvisMarch::
		description(void)
		const throw()
	{ 
		return std::string("the jarvis march algorithm to compute the convex hull of a polygon"); 
	}

	// ----------------------------------------------------------------------	

	vector<Point2D> 
		JarvisMarch::
		compute_convex_hull(vector<Point2D>* poly) 
		throw()
	{
		polygon_=*poly;
		size_of_polygon_=polygon_.size();
		size_of_convex_hull_=0;
		jm();
		
		// size_of_convex_hull_ now contains the number of the first points in polygon_ (they are in the right order) which belong to the convex hull
		vector<Point2D> convex_hull;
		for(int i=0;i<size_of_convex_hull_;i++){
			convex_hull.push_back(polygon_[i]);
		}
    	return convex_hull;
	}

	// ----------------------------------------------------------------------		
	
	void 
		JarvisMarch::
		jm() 
		throw()
	{
		int i=index_of_lowest_point();
		do
		{
			swap(size_of_convex_hull_, i);
			i=index_of_rightmost_point_from(polygon_[size_of_convex_hull_]);
			size_of_convex_hull_++;
		}
		while (i>0);
	}

	// ----------------------------------------------------------------------	

	int 
		JarvisMarch::
		index_of_lowest_point() 
		throw()
	{
		int i, min=0;
		for (i=1; i<size_of_polygon_; i++)
			if (polygon_[i].get_y()<polygon_[min].get_y() || (fabs(polygon_[i].get_y()-polygon_[min].get_y())<EPS) && polygon_[i].get_x()<polygon_[min].get_x())
				min=i;
		return min;
	}

	// ----------------------------------------------------------------------	

	int 
		JarvisMarch::
		index_of_rightmost_point_from(Point2D q) 
		throw()
	{
		int i=0, j;
		for (j=1; j<size_of_polygon_; j++)
			if (polygon_[j].relTo(q).isLess(polygon_[i].relTo(q)))
				i=j;
		return i;
	}

	// ----------------------------------------------------------------------	

	void 
		JarvisMarch::
		swap(int i, int j) 
		throw()
	{
		Point2D t=polygon_[i];
		polygon_[i]=polygon_[j];
		polygon_[j]=t;
	}	
	
}

#endif

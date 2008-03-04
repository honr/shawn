#ifndef POLYGON_H_
#define POLYGON_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "bbox_2d.h"
#include "point_2d.h"
#include "segment_2d.h"
#include "jarvis_march.h"
#include "polygon.h"
#include <vector>
#include <string>


using namespace std;
using std::vector;
using std::pair;

namespace polygon
{

/** This class implements polygons.
*
*	It provides functions for polygon topologies.
*
*/

   class Polygon
   {
	   
   public:

      Polygon();
	  /** This constructor creates a new polygon. 
	  * Note that the parameter hands over the corners of the polygon in the right order!
	  *  
	  *  \param vector<Point2D>	: the corners of the polygon to construct in the right order
	  */      
      Polygon(vector<Point2D>);      
      virtual ~Polygon();

      virtual void set_name( const std::string& ) throw();
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
 
	  /** This method adds a corner to the polygon.
	  * Note that the points of a polygon have to be in the right order!
	  *
	  *  \param Point2D : the corner to add
	  */
      virtual void push_back(Point2D) throw();
      
	  /** This method deletes a corner of the polygon.
	  *
	  *  \param Point2D : the corner to delete
	  */      
	  virtual void delete_point(Point2D) throw();				
	  
	  /** This method returns the number of corners of the polygon.
	  *
	  *  \return int	: the size of the vector polygon_
	  */ 	  
	  virtual int get_numberofpoints(void) throw();				
	  
	  /** This method returns a corners's index in the member variable polygon_.
	  *  If the corner is not part of the polygon, the method returns -1. 	
	  *
	  *  \param Point2D	: the corner to search for
	  *  \return int	: the corners's index
	  */ 	  
	  virtual int contains_point(Point2D) throw();		
	  
	  /** This method returns a point's index in a vector<Point2D>.
	  *  If the point is not part of the vector, the method returns -1. 	
	  *
	  *  \param Point2D	: the point to search for
	  *  \param vector<Point2D> : the vector where to search for the point
	  *  \return int	: the point's index
	  */ 	  
	  virtual int contains_point(Point2D, vector<Point2D>) throw();
	  
	  /** This method returns the polygon.	
	  *
	  *  \return vector<Point2D> 	: returns the member variable polygon_
	  */ 	  
	  virtual vector<Point2D> get_poly_vector(void) throw();	
	   
	  /** This method computes the segments of a polygon consisting of ordered corners and returns them in a vector.	
	  *
	  *  \return vector<Segment2D> 	: returns the computed segments of the polygon
	  */ 	  	  
	  virtual vector<Segment2D> get_segments(void) throw();		

	  /** This method computes and returns the bounding box of a polygon.	
	  *
	  *  \return Bbox2D 	: the smallest bounding box of a polygon
	  */	  
	  virtual Bbox2D getBoundingBox(void) throw();	
	  
	  /** This method returns true if a point lies inside a polygon's area and 
	  * returns false if the point lies outside.	
	  *
	  *  \param Point2D	: the point to check for
	  *  \return bool 	: true if the point lies on the bounded side of the polygon, else false
	  */		  
	  virtual bool bounded_side(Point2D) throw();	
	  
	  /** This method returns true if a point lies on a polygon's boundary else returns false.
	  *
	  *  \param Point2D	: the point to check for
	  *  \return bool 	: true if the point lies on the boundary of the polygon, else false
	  */	  
	  virtual bool on_boundary(Point2D) throw();
	  
	  /** This method returns true if the polygon is simple else returns false.
	  * A polygon is simple if the boundary of the polygon does not cross itself
	  * (all nodes of the graph induced by its segments have degree two).
	  *
	  *  \return bool 	: true if the polygon is simple else false
	  */	  
	  virtual bool is_simple() throw();							
	  
	  /** This method returns true if the polygon is empty (contains no corners) else returns false.
	  *
	  *  \return bool 	: true if the polygon is empty else false
	  */	  
	  virtual bool is_empty() throw();							
	  
	  /** This method computes and returns a polygon's convex hull.
	  *
	  *  \return Polygon : the polygon's convex hull
	  */	  
	  virtual Polygon compute_convex_hull(void) throw();			

	  /** An iterator for the corners of the polygon.
	  */
	  typedef vector<Point2D>::iterator iterator; 	
	  /** A const iterator for the corners of the polygon.
	  */	  
	  typedef vector<Point2D>::const_iterator const_iterator; 	

	  iterator begin() { return polygon_.begin(); } 
	  iterator end()   { return polygon_.end(); } 
	  const_iterator begin() const { return polygon_.begin(); } 
	  const_iterator end() const   { return polygon_.end(); } 

	  /** An iterator for the segments of the polygon.
	  */	  
	  typedef vector<Segment2D>::iterator Edge_iterator; 
	  /** A const iterator for the segments of the polygon.
	  */		  
	  typedef vector<Segment2D>::const_iterator Edge_const_iterator; 	

	  Edge_iterator e_begin() { return segments_.begin(); } 
	  Edge_iterator e_end()   { return segments_.end(); } 
	  Edge_const_iterator e_begin() const { return segments_.begin(); } 
	  Edge_const_iterator e_end() const   { return segments_.end(); } 	  

	  /** To write the polygon's elements.
	  */	  	  
	  friend std::ostream& operator << ( std::ostream&, const Polygon& )
         throw();

   private:
	  std::string name_;
	    	  
      vector<Point2D> polygon_;									// vector of vertices
      vector<Segment2D> segments_;								// vector of edges between the vertices
  	 
   };

}

#endif /*ENABLE_TOPOLOGY*/
#endif /*POLYGON_H_*/

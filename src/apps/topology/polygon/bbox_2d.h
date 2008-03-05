/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef BBOX_2D_H_
#define BBOX_2D_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "sys/vec.h"
#include <vector>

using namespace std;
using namespace shawn;

namespace polygon
{

class Bbox2D
{
	   
public:

	Bbox2D();
	~Bbox2D();
	
	Bbox2D getBoundingBox(const vector<Vec>&) throw();	// returns the bounding box of a polygon with 2-dimensional points
	
	double get_min_x(void) const throw();
	double get_max_x(void) const throw();
	double get_min_y(void) const throw();
	double get_max_y(void) const throw();
	double get_width(void) const throw();
	double get_height(void) const throw();
   
private:
 			
	double min_x_;			// left border of the bounding box
	double max_x_;			// right border of the bounding box	
	double min_y_;			// bottom of the bounding box
	double max_y_;			// top of the bounding box
	double width_;			// width of the bounding box
	double height_;			// height of the bounding box	
	
};

}

#endif
#endif /*BBOX_2D_H_*/

#ifndef utils_Garbage_h
#define utils_Garbage_h

#include "MinimalBoundingRectangle.h"
#include <vector>
#include "Contours.h"

namespace utils{

	class Garbage {
		
	 public:
		 
	     double angleTo();
	
	     double distanceTo();
	
	     utils::MinimalBoundingRectangle * boundingBox();
	
		 ~Garbage();
	     Garbage(utils::MinimalBoundingRectangle * mbr);
	     Garbage(utils::MinimalBoundingRectangle * mbr,std::vector<int> centroid);
	     Garbage(utils::MinimalBoundingRectangle * mbr,std::vector<int> centroid,Contours * contour);
	     std::vector<int> getCentroid();
	     
	     double area;
	     double perimeter;
	     	     
	     //benchmark purposes
	     bool isPredicted;
	     bool isVisualized;
	     bool isFocused;
	
	 private:
	    double angle;
	    double distance;
	    std::vector<int> centroid;
	    utils::MinimalBoundingRectangle * mbr;
	
	};

}

#endif // utils_Garbage_h

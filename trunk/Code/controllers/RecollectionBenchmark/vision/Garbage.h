#ifndef vision_Garbage_h
#define vision_Garbage_h

#include <vision/MinimalBoundingRectangle.h>
#include <vector>
#include "Contours.h"

namespace vision{

	class Garbage {
		
	 public:
		 
	     vision::MinimalBoundingRectangle * boundingBox();
	
	     Garbage(vision::MinimalBoundingRectangle * mbr);
	     Garbage(vision::MinimalBoundingRectangle * mbr,std::vector<int> centroid);
	     Garbage(vision::MinimalBoundingRectangle * mbr,std::vector<int> centroid,Contours * contour);

	     std::vector<int> getCentroid();

		 ~Garbage();
	     
	     double area;
	     double perimeter;
	     	     
	     //benchmark purposes
	     bool isPredicted;
	     bool isVisualized;
	     bool isFocused;
	
	 private:
	    std::vector<int> centroid;
	    vision::MinimalBoundingRectangle * mbr;
	
	};

}

#endif // vision_Garbage_h

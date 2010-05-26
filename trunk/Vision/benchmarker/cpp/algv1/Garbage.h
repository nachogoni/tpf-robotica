#ifndef utils_Garbage_h
#define utils_Garbage_h

#include "MinimalBoundingRectangle.h"
#include <vector>

namespace utils{

	class Garbage {
		
	 public:
		 
	     double angleTo();
	
	     double distanceTo();
	
	     utils::MinimalBoundingRectangle * boundingBox();
	
		 ~Garbage();
	     Garbage(utils::MinimalBoundingRectangle * mbr);
	     Garbage(utils::MinimalBoundingRectangle * mbr,std::vector<int> centroid);
	     std::vector<int> getCentroid();
	     
	
	 private:
	    double angle;
	    double distance;
	    std::vector<int> centroid;
	    utils::MinimalBoundingRectangle * mbr;
	
	};

}

#endif // utils_Garbage_h

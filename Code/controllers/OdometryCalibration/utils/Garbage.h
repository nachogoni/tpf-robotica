#ifndef utils_Garbage_h
#define utils_Garbage_h

#include <utils/MinimalBoundingRectangle.h>

namespace utils{

	class Garbage {
		
	 public:
	
	     double angleTo();
	
	     double distanceTo();
	
	     utils::MinimalBoundingRectangle * boundingBox();
	
	     Garbage(utils::MinimalBoundingRectangle * mbr);
	
	 private:
	    double angle;
	    double distance;
	    utils::MinimalBoundingRectangle * mbr;
	
	};

}

#endif // utils_Garbage_h

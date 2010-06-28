#ifndef utils_Garbage_h
#define utils_Garbage_h

#include <utils/MinimalBoundingRectangle.h>

namespace utils{

	class Garbage {
		
	 public:

	     utils::MinimalBoundingRectangle * boundingBox();
	
	     Garbage(utils::MinimalBoundingRectangle * mbr);

		~Garbage();
	 private:
	    double angle;
	    double distance;
	    utils::MinimalBoundingRectangle * mbr;
	
	};

}

#endif // utils_Garbage_h

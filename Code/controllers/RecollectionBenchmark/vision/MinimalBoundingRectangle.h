#ifndef vision_MinimalBoundingRectangle_h
#define vision_MinimalBoundingRectangle_h

namespace vision{

class MinimalBoundingRectangle {

 public:

     int getTopX();

     int getTopY();

     int getWidth();

     int getHeight();

	MinimalBoundingRectangle(int x,int y,int height,int width);
	
	 int x;
	 int y;
	 int height;
	 int width;


};

}

#endif // vision_MinimalBoundingRectangle_h

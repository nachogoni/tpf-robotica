#ifndef utils_MinimalBoundingRectangle_h
#define utils_MinimalBoundingRectangle_h

namespace utils{

class MinimalBoundingRectangle {

 public:

     int getTopX();

     int getTopY();

     int getWidth();

     int getHeight();

	MinimalBoundingRectangle(int x,int y,int height,int width);
	
	~MinimalBoundingRectangle();
	private:
	 int x;
	 int y;
	 int height;
	 int width;


};

}

#endif // utils_MinimalBoundingRectangle_h

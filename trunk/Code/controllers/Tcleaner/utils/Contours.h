#ifndef utils_Contours_h
#define utils_Contours_h

#include <cv.h>
#include <highgui.h>

CvSeq ** myFindContours(IplImage * image);

namespace utils {

	class Contours {
		
	 public:
	
		Contours(CvSeq * contour);
		
		void printContour(int depthLevel, CvScalar color,IplImage * dst);

		CvSeq * getContour();

/*Contour filters
 * Boolean functions determine if countours correspond
 * to desired objects
 */


/*
 * Checks that the area of the contour is beetween 
 * min_area and max_area
 * 
 */
		int areaFilter(double min_area,double max_area);

/*
 * Checks that the perimeter of the contour is beetween 
 * min_area and max_area
 * 
 */
		int perimeterFilter(double min_per,double max_per);

/*
 * Checks that the minimum bounding box covering the contour
 * has a width-height ratio similar to a rectangle
 * 
 */
		int rectangularAspectFilter(double minWidthHeightRatio, double maxWidthHeightRatio );

/*
 * Checks that the contour area cover at least minAreaRatio
 * of the minimum bounding box. This way, we filter irregular shapes.
 * 
 */
		int boxAreaFilter(double minAreaRatio);

/*
 * Compares the histogram of a model object with one taken from 
 * the actual minimum boundingbox containing the contour. min determines
 * the minimum value of similiarity they must comply.
 */
		int histogramMatchingFilter(IplImage * src, CvHistogram * testImageHistogram,int h_bins,int s_bins,double min);

		private :
			CvSeq * c;
	};

}

#endif

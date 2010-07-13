#ifndef utils_Contours_h
#define utils_Contours_h

#include <cv.h>
#include <highgui.h>
#include <vector>

CvSeq * myFindContours(IplImage * image);
CvSeq * getPolygon(CvSeq * aContour);
void traversePoints(CvSeq * contour,IplImage * src);
CvPoint * getLongestEdge(CvSeq * points,CvPoint * excludedHead,CvPoint * excludedTail);
double edgeLength(CvPoint tail,CvPoint head);
bool equalsCvPoint(CvPoint * a, CvPoint * b);
bool equalsEdge(CvPoint * head, CvPoint * tail,CvPoint * head2, CvPoint * tail2);
CvPoint * getLongestEdge2(CvSeq * points,CvPoint * excludedHead,CvPoint * excludedTail,int * shorterEdges);

const double PI = 3.141592;


namespace utils {

	class Contours {
		
	 public:
	
		Contours(CvSeq * contour);
		
		Contours(CvSeq * contour,CvRect window);
		
		void printContour(int depthLevel, CvScalar color,IplImage * dst);
		
		double getArea();
		
		double getPerimeter();

		CvSeq * getContour();
		
		double getEccentricity();
		
		

	

/*Contour filters
 * Boolean functions determine if countours correspond
 * to desired objects
 */

		int numberOfPointsFilter(int minNumberOfPoints);

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
/*
 * Calculates the centroid of the contour
 */
		std::vector<int> getCentroid();
		
/*
 * Checks if contour is a glass
 */
	int vasoFilter();
	
	
	int platoFilter();

		private :
			CvSeq * c;
			int x;
			int y;
	};




}

#endif

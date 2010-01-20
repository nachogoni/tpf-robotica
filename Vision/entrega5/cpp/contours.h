

#include <cv.h>
#include <highgui.h>
#include <stdio.h>



CvSeq ** 
findContours(IplImage * image);


void 
printContour(CvSeq * contour, int depthLevel, CvScalar color,IplImage * dst);

/*Contour filters
 * Boolean functions determine if countours correspond
 * to desired objects
 */


/*
 * Checks that the area of the contour is beetween 
 * min_area and max_area
 * 
 */
int
areaFilter(CvSeq * contour,double min_area,double max_area);

/*
 * Checks that the perimeter of the contour is beetween 
 * min_area and max_area
 * 
 */
int
perimeterFilter(CvSeq * contour,double min_per,double max_per);

/*
 * Checks that the minimum bounding box covering the contour
 * has a width-height ratio similar to a rectangle
 * 
 */
int
rectangularAspectFilter(CvSeq * contour,double minWidthHeightRatio,
double maxWidthHeightRatio );

/*
 * Checks that the contour area cover at least minAreaRatio
 * of the minimum bounding box. This way, we filter irregular shapes.
 * 
 */
int
boxAreaFilter(CvSeq * contour,double minAreaRatio);

/*
 * Compares the histogram of a model object with one taken from 
 * the actual minimum boundingbox containing the contour. min determines
 * the minimum value of similiarity they must comply.
 */
int 
histogramMatchingFilter(IplImage * src,CvSeq * contour,
CvHistogram * testImageHistogram,int h_bins,int s_bins,double min);




#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>



CvSeq ** 
findContours(IplImage * image);


void 
printContour(CvSeq * contour, int depthLevel, CvScalar color,IplImage * dst);

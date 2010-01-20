#include <cv.h>
#include <highgui.h>
#include <stdio.h>



CvHistogram * 
getHShistogramFromHS(IplImage* hChannel,IplImage* sChannel,int h_bins,int s_bins);


CvHistogram * 
getHShistogramFromRGB(IplImage* src,int h_bins,int s_bins);

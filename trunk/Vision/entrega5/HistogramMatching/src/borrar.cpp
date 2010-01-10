	//      compareByBackProjection.cpp
//      
//      Copyright 2010 dnul <dnul@alu.itba.edu.ar>
//      


#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include "2dhist.c"

int main(int argc, char** argv)
{
	//windows
	cvNamedWindow("src",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("test",CV_WINDOW_AUTOSIZE);
	
	//image to search
    IplImage* src;
    //test Image to find
    IplImage* test;
    
    //video to search
    CvCapture* capture;
	
	capture=cvCreateFileCapture( argv[1]);
	printf("%d",capture);
    src=cvQueryFrame(capture);
	cvShowImage("test",src);

	cvWaitKey();
	return 0;
}


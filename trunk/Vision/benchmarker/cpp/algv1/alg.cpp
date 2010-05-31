
#include <stdio.h>
#include "Contours.h"
#include "GarbageRecognition.h"
#include "Garbage.h"	

#define _RED  cvScalar (0, 0, 255, 0)
#define _GREEN cvScalar (0, 255, 0, 0)

int main(int argc, char** argv)
{
	cvNamedWindow("output",CV_WINDOW_AUTOSIZE);
	
	//video to analyze
    CvCapture* capture;
	
	//frame from video
	IplImage * src;
	//object model
	IplImage * model;
	
	if( argc != 2 || (capture=cvCreateFileCapture( argv[1]))== 0 ){
		perror("Invalid capture");
		return 0;
	}
	
	utils::GarbageRecognition * gr= new utils::GarbageRecognition();
	
	
	while(src=cvQueryFrame(capture)){
		std::list<utils::Garbage*> glist=gr->getGarbageList(src);
		
		
		
		for (std::list<utils::Garbage*>::iterator itRes = glist.begin(); itRes != glist.end(); itRes++){
			utils::MinimalBoundingRectangle * boundingRect=(*itRes)->boundingBox();
			cvRectangle(src,cvPoint(boundingRect->x,boundingRect->y),
								cvPoint(boundingRect->x+boundingRect->width,
								boundingRect->y+boundingRect->height),
								_GREEN,1,8,0);
			
						}
						
		cvShowImage("output",src);
		cvWaitKey(1000/20);
		
	}
	
	
	return 0;
}

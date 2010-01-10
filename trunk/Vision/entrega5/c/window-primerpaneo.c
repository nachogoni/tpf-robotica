//      window-primerpaneo.c
//      
//      Copyright 2010 dnul <dnul@alu.itba.edu.ar>
//      

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include "contours.h"


#define THRESHOLD_VALUE 240
#define MORPH_KERNEL_SIZE 2
#define MORPH_DILATE_ITER 2
#define MORPH_ERODE_ITER 2

#define _RED  cvScalar (0, 0, 255, 0)
#define _GREEN cvScalar (0, 255, 0, 0)



int main(int argc,char * argv[]){
	
	//video to analyze
    CvCapture* capture;
	
	//frame from video
	IplImage * src;
	
	if( argc == 2 && (capture=cvCreateFileCapture( argv[1]))!= 0 ){
		perror("Invalid capture");
		return 0;
	}
	
	int frameWidth=cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH);
	int frameHeight=cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT);
	
	
	//video image size
	CvSize srcSize = cvSize(frameWidth,frameHeight);
	

	//images for HSV conversion
	IplImage* hsv = cvCreateImage( srcSize, 8, 3 );
	IplImage* h_plane = cvCreateImage( srcSize, 8, 1 );
	IplImage* s_plane = cvCreateImage( srcSize, 8, 1 );
	IplImage* v_plane = cvCreateImage( srcSize, 8, 1 );
	
	//Image for thresholding
	IplImage * threshImage=cvCreateImage(srcSize,8,1);
	
	//image for equalization
	IplImage * equalizedImage=cvCreateImage(srcSize,8,1);
	
	//image for Morphing operations(Dilate-erode)
	IplImage * morphImage=cvCreateImage(srcSize,8,1);
	
	//image for image smoothing
	IplImage * smoothImage=cvCreateImage(srcSize,8,1);
	
	//image for contour-finding operations
	IplImage * contourImage=cvCreateImage(srcSize,8,1);
	
	
	int frameCounter=1;
	int cont_index=0;
	
	//convolution kernel for morph operations
	IplConvKernel* element;
	
	CvRect boundingRect;
	
	//contours
	CvSeq ** contours;
	
	//Main loop
	while(src=cvQueryFrame(capture)){
		
		frameCounter++;
		
		//convert image to hsv
		cvCvtColor( src, hsv, CV_BGR2HSV );
		cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
		
		//equalize Saturation Channel image
		cvEqualizeHist(s_plane,equalizedImage);
		
		//threshold the equalized Saturation channel image
		cvThreshold(equalizedImage,threshImage,THRESHOLD_VALUE,255,
		CV_THRESH_BINARY_INV);
		
		//apply morphologic operations
		element = cvCreateStructuringElementEx( MORPH_KERNEL_SIZE*2+1, 
			MORPH_KERNEL_SIZE*2+1, MORPH_KERNEL_SIZE, MORPH_KERNEL_SIZE, 
			CV_SHAPE_RECT, NULL);
			
		cvDilate(threshImage,morphImage,element,MORPH_DILATE_ITER);
        cvErode(morphImage,morphImage,element,MORPH_ERODE_ITER);
	
		//apply smooth gaussian-filter
        cvSmooth(morphImage,smoothImage,CV_GAUSSIAN,3,0,0,0);
		
		contours=findContours(smoothImage);
		
		while(contours[cont_index]!=NULL){
			printContour(contours[cont_index],3,cvScalar(127,127,0,0),
				contourImage);
			
			//get contour bounding box
			boundingRect=cvBoundingRect(contours[cont_index],0);
			cvRectangle(contourImage,cvPoint(boundingRect.x,boundingRect.y),
					cvPoint(boundingRect.x+boundingRect.width,
					boundingRect.y+boundingRect.height),
					_GREEN,1,8,0);
			
			cont_index++;
		}
		
		

	} 
	
		
}
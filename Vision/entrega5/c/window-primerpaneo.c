//      window-primerpaneo.c
//      
//      Copyright 2010 dnul <dnul@alu.itba.edu.ar>
//      

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include "contours.h"
#include "histogram.h"


// image preprocessing values
#define THRESHOLD_VALUE 240
#define MORPH_KERNEL_SIZE 2
#define MORPH_DILATE_ITER 2
#define MORPH_ERODE_ITER 2

#define _RED  cvScalar (0, 0, 255, 0)
#define _GREEN cvScalar (0, 255, 0, 0)



//contour filters constants
#define MINCONTOUR_AREA 1000
#define MAXCONTOUR_AREA 10000
#define BOXFILTER_TOLERANCE 0.55
#define MINCONTOUR_PERIMETER 80
#define MAXCONTOUR_PERIMETER 1000
#define CONTOUR_RECTANGULAR_MIN_RATIO 1.2
#define CONTOUR_RECTANGULAR_MAX_RATIO 3.0
#define HIST_S_BINS 8
#define HIST_H_BINS 8
#define HIST_MIN 0.7



int main(int argc,char * argv[]){
	
	//
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
	
	
	//image for the histogram-based filter
	//could be a parameter
	model=cvLoadImage("../images/colilla-sinBlanco.png",1);
	CvHistogram * testImageHistogram=getHShistogramFromRGB(model,HIST_H_BINS,HIST_S_BINS);
	
	//~ int frameWidth=cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH);
	//~ int frameHeight=cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT);
	
	
	
	//gets a frame for setting  image size
	//CvSize srcSize = cvSize(frameWidth,frameHeight);
	src=cvQueryFrame(capture);
	CvSize srcSize = cvGetSize(src);

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
	IplImage * contourImage=cvCreateImage(srcSize,8,3);
	
	
	int frameCounter=1;
	int cont_index=0;
	
	//convolution kernel for morph operations
	IplConvKernel* element;
	
	CvRect boundingRect;
	
	//contours
	CvSeq * contours;
	CvSeq * aContour;
	CvSeq * polygonContour;
	
	//Main loop
	while(src=cvQueryFrame(capture)){
		
		frameCounter++;
		printf("frame number:%d\n",frameCounter);
		
		//convert image to hsv
		cvCvtColor( src, hsv, CV_BGR2HSV );
		cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
		
		//equalize Saturation Channel image
		cvEqualizeHist(s_plane,equalizedImage);
		
		//threshold the equalized Saturation channel image
		cvThreshold(equalizedImage,threshImage,THRESHOLD_VALUE,255,
		CV_THRESH_BINARY);
		
		//apply morphologic operations
		element = cvCreateStructuringElementEx( MORPH_KERNEL_SIZE*2+1, 
			MORPH_KERNEL_SIZE*2+1, MORPH_KERNEL_SIZE, MORPH_KERNEL_SIZE, 
			CV_SHAPE_RECT, NULL);
			
		cvDilate(threshImage,morphImage,element,MORPH_DILATE_ITER);
        cvErode(morphImage,morphImage,element,MORPH_ERODE_ITER);
	
		//apply smooth gaussian-filter
        cvSmooth(morphImage,smoothImage,CV_GAUSSIAN,3,0,0,0);
		
		//get all contours
		contours=findContours(smoothImage);
		
		cont_index=0;
		cvCopy(src,contourImage,0);
		aContour=contours;
		
		while(contours!=NULL){
			aContour=contours;
			polygonContour=getPolygon(aContour);
			
			//apply filters
			if(perimeterFilter(polygonContour,MINCONTOUR_PERIMETER,
				MAXCONTOUR_PERIMETER) &&
				//areaFilter(aContour,MINCONTOUR_AREA,MAXCONTOUR_AREA) &&
				rectangularAspectFilter(polygonContour,CONTOUR_RECTANGULAR_MIN_RATIO,
					CONTOUR_RECTANGULAR_MAX_RATIO) &&
				boxAreaFilter(aContour,BOXFILTER_TOLERANCE) &&
				histogramMatchingFilter(src,polygonContour,testImageHistogram,
					HIST_H_BINS,HIST_S_BINS,HIST_MIN)){

					//if passed filters
				
					printContour(polygonContour,3,cvScalar(127,127,0,0),
						contourImage);
				
					//get contour bounding box
					boundingRect=cvBoundingRect(polygonContour,0);
					cvRectangle(contourImage,cvPoint(boundingRect.x,boundingRect.y),
							cvPoint(boundingRect.x+boundingRect.width,
							boundingRect.y+boundingRect.height),
							_GREEN,1,8,0);
				}
				
			cont_index++;
			cvReleaseMemStorage( &polygonContour->storage );
			contours=contours->h_next;
		}
		
		cvShowImage("output",contourImage);
		cvWaitKey(0);

	} 
	
		
}

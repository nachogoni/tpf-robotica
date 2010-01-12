/*
 *      contours.c
 *      
 *      Copyright 2010 Diego Nul <dnul@alu.itba.edu.ar>
 *      
 *      This library takes care of contour finding and filtering
 * 		using openCV library
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "contours.h"
#include <stdio.h>


#define PER_TOLERANCE 50


/* findContours
 * Given a binary image returns a sequence of contours found in 
 * the image.
 */
CvSeq ** 
findContours(IplImage * image){
	
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq * contours;
	CvSeq * aContour;
	CvSeq ** polygonArray=NULL;
	int polyIndex=0;
	
	int numberOfContours;
	double contourPerimeter;
    
	//find all contours
	numberOfContours=cvFindContours( image, storage, &contours, 
			sizeof(CvContour),CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, 
			cvPoint(0,0) );
			
	if( (polygonArray=malloc(sizeof(CvSeq*)*(numberOfContours+1)))==NULL){
		perror("Out of memory");
		return NULL;
	} 
		
	while(contours!=NULL){
			contourPerimeter=cvContourPerimeter(contours);
			aContour=cvApproxPoly (contours, sizeof(CvContour),
				storage,CV_POLY_APPROX_DP, contourPerimeter/PER_TOLERANCE
				, 0);
			
			//add polygon to return array
			polygonArray[polyIndex]=aContour;
			// take the next contour
			contours = contours->h_next;
			polyIndex++;
	}
	
    		
	polygonArray[polyIndex]=NULL;
	cvReleaseMemStorage( &storage );
	return polygonArray;
}

/* 
 * Prints a contour on a dst Image.
 */
void printContour(CvSeq * contour, int depthLevel, CvScalar color,IplImage * dst){
	
	cvDrawContours( dst, contour, CV_RGB(255,0,0), CV_RGB(0,255,0), 
		depthLevel, 3, CV_AA, cvPoint(0,0) );
}


/*Contour Filters
 *
 */

int
areaFilter(CvSeq * contour,double min_area,double max_area){
	double area;
	area=fabs(cvContourArea(contour,CV_WHOLE_SEQ));
	
	return area> min_area && area<max_area;
	
}

int
perimeterFilter(CvSeq * contour,double min_per,double max_per){
	
	double per;
	per=cvContourPerimeter(contour);
	
	return per> min_per && per<max_per;	
	
}

int
rectangularAspectFilter(CvSeq * contour,double minWidthHeightRatio,
double maxWidthHeightRatio ){
	CvBox2D box;
	CvMemStorage* mem = cvCreateMemStorage(0);
	int ret;
	
	box=cvMinAreaRect2(contour,mem);
	
	if( (box.size.width > minWidthHeightRatio* box.size.height) &&
	 (box.size.width < (maxWidthHeightRatio)* box.size.height) || 
	 (box.size.height > minWidthHeightRatio* box.size.width) &&
     (box.size.height < (maxWidthHeightRatio)*box.size.width)){
		  ret=1;
	 }
	 else{
		ret=0;	
	}
	 
	 cvReleaseMemStorage( &mem );
	 return ret;
}

int
boxAreaFilter(CvSeq * contour,double minAreaRatio){
	CvBox2D box;
	CvMemStorage* mem = cvCreateMemStorage(0);
	double boxArea,contourArea,dif,ret;
	
	box=cvMinAreaRect2(contour,mem);
	boxArea=box.size.width*box.size.height;
	contourArea=fabs(cvContourArea(contour,CV_WHOLE_SEQ));
	dif=boxArea-contourArea;
	ret=(1- (dif/boxArea));
        
     
    cvReleaseMemStorage( &mem );
	return ret > minAreaRatio;
}

int
histogramMatchingFilter(IplImage * frame,CvHistogram * testImageHistogram,
double min){
	//~ CvBox2d box;
	//~ CvMemStorage* mem = cvCreateMemStorage(0);
	//~ 
	//~ 
	//~ box=cvMinAreaRect2(contour,mem);
	//~ void cvSetImageROI( IplImage* image, CvRect rect );
//~ 
	//~ 
	//~ 
	//~ 
	
	
	return 0;
	
	


}




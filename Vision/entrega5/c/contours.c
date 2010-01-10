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
			
	if( (polygonArray=malloc(sizeof(CvSeq*)*(numberOfContours+1)))==NULL)
		perror("Out of memory");
		return NULL;

		
	while(contours){
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
	
    		
	polygonArray[polyIndex+1]=NULL;
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



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
#include "histogram.h"
#include <stdio.h>


#define PER_TOLERANCE 50


/* findContours
 * Given a binary image returns a sequence of contours found in 
 * the image.
 */
CvSeq * 
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
			
	return contours;

	}
	
   
CvSeq * 
getPolygon(CvSeq * aContour)
	{
		CvMemStorage* storage = cvCreateMemStorage(0);
		double contourPerimeter;
		CvSeq* aPolyContour;
		
		contourPerimeter=cvContourPerimeter(aContour);
		aPolyContour=cvApproxPoly (aContour, sizeof(CvContour),
				storage,CV_POLY_APPROX_DP, contourPerimeter/PER_TOLERANCE
				, 0);
		
		
		
		return aPolyContour;
	
	
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
	
	if (mem == NULL)
		printf("\nEs null");
	
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

/*
 * @src Frame image
 * @contour  contour processing
 * @testImageHistogram histogram from model image
 * @h_bins number of hue bins
 * @s_bins number of sat bins 
 * @min minimum similarity to achieve when comparing histograms
 */
int
histogramMatchingFilter(IplImage * src,CvSeq * contour,
CvHistogram * testImageHistogram,int h_bins,int s_bins,
double min){
	CvRect box;
	CvMemStorage* mem = cvCreateMemStorage(0);
	
	double val;
	
	
	//get contour bounding box
	box=cvBoundingRect(contour,0);
	
	printf("box x:%d y:%d \n",box.x,box.y);
	
	IplImage * src_bbox=cvCreateImage(cvSize(box.width,box.height),src->depth,src->nChannels);
	
	//gets subimage bounded by box
    cvGetSubArr( src,(CvMat*)src_bbox, box );

	printf("armo la subImagen \n");
	//gets subimage histogram
	CvHistogram* hist=getHShistogramFromRGB(src_bbox,h_bins,s_bins);
	//compares with object histogram
	val=cvCompareHist(hist,testImageHistogram,CV_COMP_BHATTACHARYYA);
	
	cvReleaseHist(&hist);
	cvReleaseImage(&src_bbox);
	
	printf("%g\n",val);
	
	return (val<min);
}
//30,32,10





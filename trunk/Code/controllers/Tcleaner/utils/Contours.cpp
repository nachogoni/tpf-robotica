/*
 *      contours.c
 *      
 *      Copyright 2010 Diego Nul <dnul@alu.itba.edu.ar>
 *      
 *      This library takes care of contour finding and filtering
 * 		using openCV library
 */

#include <utils/Contours.h>
#include <utils/Histogram.h>
#include <stdio.h>

#define PER_TOLERANCE 50

/* findContours
 * Given a binary image returns a sequence of contours found in
 * the image.
 */

CvSeq ** findContours(IplImage * image){

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

	if( (polygonArray=(CvSeq**)malloc(sizeof(CvSeq*)*(numberOfContours+1)))==NULL){
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


namespace utils{

Contours::Contours(CvSeq * contour){
	this->c = contour;
}

/* 
 * Prints a contour on a dst Image.
 */
void Contours::printContour(int depthLevel, CvScalar color,IplImage * dst){
	
	cvDrawContours( dst, this->c, CV_RGB(255,0,0), CV_RGB(0,255,0), 
		depthLevel, 3, CV_AA, cvPoint(0,0) );
}


/*Contour Filters
 *
 */

int Contours::areaFilter(double min_area,double max_area){
	double area;
	area=fabs(cvContourArea(this->c,CV_WHOLE_SEQ));
	
	return area> min_area && area<max_area;
	
}

int Contours::perimeterFilter(double min_per,double max_per){
	
	double per;
	per=cvContourPerimeter(this->c);
	
	return per> min_per && per<max_per;	
	
}

int Contours::rectangularAspectFilter(double minWidthHeightRatio, double maxWidthHeightRatio ){
	CvBox2D box;
	CvMemStorage* mem = cvCreateMemStorage(0);
	int ret;
	
	if (mem == NULL)
		printf("\nEs null");
	
	box=cvMinAreaRect2(this->c,mem);
	
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

int Contours::boxAreaFilter(double minAreaRatio){
	CvBox2D box;
	CvMemStorage* mem = cvCreateMemStorage(0);
	double boxArea,contourArea,dif,ret;
	
	box=cvMinAreaRect2(this->c,mem);
	boxArea=box.size.width*box.size.height;
	contourArea=fabs(cvContourArea(this->c,CV_WHOLE_SEQ));
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
int Contours::histogramMatchingFilter(IplImage * src, CvHistogram * testImageHistogram,int h_bins,int s_bins, double min){
	CvRect box;
	CvMemStorage* mem = cvCreateMemStorage(0);
	
	double val;
	
	
	//get contour bounding box
	box=cvBoundingRect(this->c,0);
	
	printf("box x:%d y:%d \n",box.x,box.y);
	
	IplImage * src_bbox=cvCreateImage(cvSize(box.width,box.height),src->depth,src->nChannels);
	
	//gets subimage bounded by box
    cvGetSubArr( src,(CvMat*)src_bbox, box );

	printf("armo la subImagen \n");
	//gets subimage histogram
	utils::Histogram * h = new Histogram(h_bins,s_bins);
	CvHistogram* hist = h->getHShistogramFromRGB(src_bbox);
	//compares with object histogram
	val=cvCompareHist(hist,testImageHistogram,CV_COMP_BHATTACHARYYA);
	
	cvReleaseHist(&hist);
	cvReleaseImage(&src_bbox);
	delete h;
	printf("%g\n",val);
	
	return (val<min);
}
//30,32,10

CvSeq * Contours::getContour(){
	return this->c;
}

}

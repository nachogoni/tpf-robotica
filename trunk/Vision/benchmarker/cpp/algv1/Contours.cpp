/*
 *      contours.c
 *      
 *      
 *      This library takes care of contour finding and filtering
 * 		using openCV library
 */

#include "Contours.h"
#include "Histogram.h"
#include <stdio.h>
#include <vector>
#include <cv.h>


#define PER_TOLERANCE 500



/* findContours
 * Given a binary image returns a sequence of contours found in
 * the image.
 */

CvSeq * myFindContours(IplImage * image){	
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq * contours;

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

namespace utils{

Contours::Contours(CvSeq * contour){
	this->c = contour;
	CvPoint * p=CV_GET_SEQ_ELEM(CvPoint ,contour,0);
	this->x=p->x;
	this->y=p->y;
}

//accept window for configuring perimeter and area filters
Contours::Contours(CvSeq * contour,CvRect window){
	this->c = contour;
	CvPoint * p=CV_GET_SEQ_ELEM(CvPoint ,contour,0);
	this->x=p->x + window.x;
	this->y=p->y + window.y;
}

int getPointZone(int x, int y){
	int zone;
	
	if(y>320){
		zone=1;
	}else if(y<320 && y>160){
		zone=2;
	}else if(y<160 && y>80){
		zone=3;
	}else
		zone=4;
		
	return zone;
}




/* 
 * Prints a contour on a dst Image.
 */
void Contours::printContour(int depthLevel, CvScalar color,IplImage * dst){
	
	CvFont font;
	int line_type=CV_AA;
	
	char * a=(char *) malloc(20);
	char * b=(char *) malloc(20);
	char * c=(char *) malloc(20);
	char * d=(char *) malloc(20);
	
	cvDrawContours( dst, this->c, CV_RGB(255,0,0), CV_RGB(0,255,0), 
		depthLevel, 3, CV_AA, cvPoint(0,0) );

	std::vector<int> centroid=this->getCentroid();
	CvPoint pt2=cvPoint(centroid[0]+5,centroid[1]+5);
	CvPoint pt3=cvPoint(centroid[0]+5,centroid[1]+15);
	CvPoint pt4=cvPoint(centroid[0]+5,centroid[1]+25);
	CvPoint pt5=cvPoint(centroid[0]+5,centroid[1]+35);
	sprintf(a,"per: %g",this->getPerimeter());
	sprintf(b,"zone: %d",getPointZone(this->x,this->y));
	sprintf(c,"area: %g",this->getArea());
	//~ sprintf(d,"convex: %d",cvCheckContourConvexity(this->c));
	
	cvInitFont( &font, CV_FONT_HERSHEY_COMPLEX, 0.5, 0.5, 0.0,0.5, line_type );
	cvPutText( dst, a, pt2, &font, CV_RGB(255,255,0));
	cvPutText( dst, c, pt3, &font, CV_RGB(255,255,0));
	cvPutText( dst, b, pt4, &font, CV_RGB(255,255,0));
	cvPutText( dst, d, pt5, &font, CV_RGB(255,255,0));

	//~ free(a);
}


/*Contour Filters
 *
 */

int Contours::areaFilter(double min_area,double max_area){
	double area;
	area=this->getArea();
	 
	
	int zone=getPointZone(this->x,this->y);
	
	
	double  minAreaByZone[]={0,100,40,20,10};
	//double  maxAreaByZone[]={0,800,400,200,100};
	double  maxAreaByZone[]={0,800,550,200,50};
	
	//~ return area>minAreaByZone[zone] && area<maxAreaByZone[zone];
	return area>50 && area<800;
	
}

int Contours::perimeterFilter(double min_per,double max_per){
	
	double per;
	per=this->getPerimeter();
		
	//double  maxPerimeterByZone[]={0,300,300,75,50};
	double  minPerimeterByZone[]={0,50,15,10,10};
	double  maxPerimeterByZone[]={0,300,200,100,50};
	
	int zone=getPointZone(this->x,this->y);
	
	return per>10 && per<800;
	//~ return per>minPerimeterByZone[zone] &&  per< maxPerimeterByZone[zone];
	
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
	contourArea=this->getArea();
	dif=boxArea-contourArea;
	ret=(1- (dif/boxArea));
	
	int zone=getPointZone(this->x,this->y);
	
	double minAreaRatioByZone[]={0,0.6,0.55,0.55,0.55};
        
    cvReleaseMemStorage( &mem );
	//~ return ret > minAreaRatioByZone[zone];
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
	
	//printf("box x:%d y:%d \n",box.x,box.y);
	
	IplImage * src_bbox=cvCreateImage(cvSize(box.width,box.height),src->depth,src->nChannels);
	
	//gets subimage bounded by box
    cvGetSubArr( src,(CvMat*)src_bbox, box );

	//gets subimage histogram
	utils::Histogram * h = new Histogram(h_bins,s_bins);
	CvHistogram* hist = h->getHShistogramFromRGB(src_bbox);
	//compares with object histogram
	val=cvCompareHist(hist,testImageHistogram,CV_COMP_BHATTACHARYYA);
	
	cvReleaseHist(&hist);
	cvReleaseImage(&src_bbox);
	cvReleaseMemStorage(&mem);
	delete h;
	
	return (val<min);
}
//30,32,10

std::vector<int> Contours::getCentroid(){
	 std::vector<int> vec(2);
	
	 CvMoments * myMoments = (CvMoments*)malloc( sizeof(CvMoments) );	 
	 cvMoments( this->c, myMoments );
	 vec[0]=(int) myMoments->m10/myMoments->m00;
	 vec[1]=(int) myMoments->m01/myMoments->m00;
	 
	 free(myMoments);
	 return vec;
}

CvSeq * Contours::getContour(){
	return this->c;
}

double Contours::getArea(){
	return fabs(cvContourArea(this->c,CV_WHOLE_SEQ));
}
		
double Contours::getPerimeter(){
	return cvContourPerimeter(this->c);
	
}

//~ CvHistogram * Contours::getPGH(){
	//~ int dims[] = {8, 8};
	//~ float range[] = {-180, 180, -100, 100};
	//~ float *ranges[] = {&range[0], &range[2]}; 	
	//~ cvHistogram * hist;
	//~ cvCalcPGH( const CvSeq* contour, CvHistogram* hist );
	//~ hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );


//~ double pghMatchShapes(CvSeq *shape1, CvSeq *shape2) {
	//~ int dims[] = {8, 8};
	//~ float range[] = {-180, 180, -100, 100};
	//~ float *ranges[] = {&range[0], &range[2]};
    //~ CvHistogram* hist1 = cvCreateHist(2, dims, CV_HIST_ARRAY, ranges, 1);
    //~ CvHistogram* hist2 = cvCreateHist(2, dims, CV_HIST_ARRAY, ranges, 1);
	//~ cvCalcPGH(shape1, hist1);
    //~ cvCalcPGH(shape2, hist2);
	//~ cvNormalizeHist(hist1, 100.0f);
	//~ cvNormalizeHist(hist2, 100.0f);
    //~ double corr = cvCompareHist(hist1, hist2, CV_COMP_BHATTACHARYYA);
    //~ cvReleaseHist(&hist1);
    //~ cvReleaseHist(&hist2);
	//~ return corr;
//~ }
}

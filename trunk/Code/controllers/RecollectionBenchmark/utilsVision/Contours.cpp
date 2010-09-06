/*
 *      contours.cpp
 *      
 *      
 *      This library takes care of contour finding and filtering
 * 		using openCV library
 */

#include "Contours.h"
#include <stdio.h>
#include <vector>
#include <cv.h>

#define PER_TOLERANCE 50
#define POLYGON_MINPOINTS 4


/* myFindContours
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


/*Test function to traverse all edges ina given polygon
 
void 
traversePoints(CvSeq * contour,IplImage * img){
		CvSeq * points=contour;
		CvPoint *a,*b,*maxa,*maxb;
		double delta,maxDelta;
		CvFont font;
		int maxIndex,i;
		char * text;
		
		CvPoint * longest=getLongestEdge(points);
		CvPoint * sndLongest=getLongestEdge(points,&longest[0],&longest[1]);
		
		double l1=edgeLength(longest[0],longest[1]);
		double l2=edgeLength(sndLongest[0],sndLongest[1]);

		//~ cvLine( img, *lasta,*lastb, CV_RGB(0,255,0),10,10);
		
		cvLine( img, longest[0],longest[1], CV_RGB(0,255,0),10,10);		
		cvLine( img, sndLongest[0],sndLongest[1], CV_RGB(255,255,0),2,2);		
		
		printf("relacion entre semiejes %g\n",l2/l1);
		printf(" pendientes cruzadas %d\n", 
			isIncreasing(&longest[0],&longest[1])!=isIncreasing(&sndLongest[0],&sndLongest[1]));
		cvShowImage("lines",img);
		
}
*/

namespace utils{

Contours::Contours(CvSeq * contour){
	this->c = contour;
	CvPoint * p=CV_GET_SEQ_ELEM(CvPoint ,contour,0);
	this->x=p->x;
	this->y=p->y;
	this->per=-1;
	this->area=-1;
}



/*accept window for configuring perimeter and area filters
 * if set by zone
 * */
Contours::Contours(CvSeq * contour,CvRect window){
	this->c = contour;
	CvPoint * p=CV_GET_SEQ_ELEM(CvPoint ,contour,0);
	this->x=p->x + window.x;
	this->y=p->y + window.y;
	this->per=-1;
	this->area=-1;

}


/*
 * Returns the corresponding zone of a given coordinate
 * Currently hard-coded for 640x480
 */
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
 * Prints a contour on a dst Image. Used for debugging.
 * prints text at the side of a contour.
 * depthLevel sets the level in the contour tree(to include/exclue holes)
 */
void Contours::printContour(int depthLevel, CvScalar color,IplImage * dst){
	
	CvFont font;
	int line_type=CV_AA;
	
	char * a=(char *) malloc(20);
	char * b=(char *) malloc(20);
	char * c=(char *) malloc(20);
	char * d=(char *) malloc(20);
	char * e=(char *) malloc(20);
	
	
	cvDrawContours( dst, this->c, CV_RGB(255,0,0), CV_RGB(0,255,0), 
		depthLevel, 3, CV_AA, cvPoint(0,0) );
	
	CvMemStorage* mem = cvCreateMemStorage(0);
	CvBox2D box=cvMinAreaRect2(this->c,mem);
	
	
	//~ traversePoints(this->c);

	std::vector<int> centroid=this->getCentroid();
	CvPoint pt2=cvPoint(centroid[0]+5,centroid[1]+5);
	CvPoint pt3=cvPoint(centroid[0]+5,centroid[1]+15);
	CvPoint pt4=cvPoint(centroid[0]+5,centroid[1]+25);
	CvPoint pt5=cvPoint(centroid[0]+5,centroid[1]+35);
	CvPoint pt6=cvPoint(centroid[0]+5,centroid[1]+45);
	sprintf(a,"per: %g",this->getPerimeter());
	sprintf(b,"zone: %d",getPointZone(this->x,this->y));
	sprintf(c,"area: %g",this->getArea());
	sprintf(d,"ecc: %g",this->getPerimeter()*this->getPerimeter()/this->getArea());
	//~ sprintf(d,"boxArea: %g",(double) this->getArea()/(box.size.width*box.size.height));
	
	cvInitFont( &font, CV_FONT_HERSHEY_COMPLEX, 0.5, 0.5, 0.0,0.5, line_type );
	cvPutText( dst, a, pt2, &font, CV_RGB(255,255,0));
	cvPutText( dst, c, pt3, &font, CV_RGB(255,255,0));
	cvPutText( dst, b, pt4, &font, CV_RGB(255,255,0));
	cvPutText( dst, d, pt5, &font, CV_RGB(255,255,0));

	//~ free(a);
	cvReleaseMemStorage(&mem);
}

/*Contour Filters
 * These filters determine if a given contour is an object of interest
 */

/*area filter - sets minimum and maximum thresholds for the 
 * area of a given contour */
int Contours::areaFilter(double min_area,double max_area){
	double area;
	area=this->getArea();
	 
	int zone=getPointZone(this->x,this->y);
	
	/* Use this to filter area by zone
	double  minAreaByZone[]={0,100,40,20,10};
	double  maxAreaByZone[]={0,800,550,200,50};
	return area>minAreaByZone[zone] && area<maxAreaByZone[zone];
	*/

	return area>min_area && area<max_area;	
}

/* Perimeter filter - sets minimum and maximum thresholds for the 
 * area of a given contour */
int Contours::perimeterFilter(double min_per,double max_per){
	
	double per;
	per=this->getPerimeter();
		
	
	/*Use this to filter perimeter by zone
	double  minPerimeterByZone[]={0,50,15,10,10};
	double  maxPerimeterByZone[]={0,300,200,100,50};
	int zone=getPointZone(this->x,this->y);
	return per>minPerimeterByZone[zone] &&  per< maxPerimeterByZone[zone];
	*/
	
	return per>min_per && per<max_per;
}

/* rectangular aspect filter - checks that a contour bounding rectangle
 * has certain width/height proportions */
int Contours::rectangularAspectFilter(double minWidthHeightRatio, double maxWidthHeightRatio ){
	CvBox2D box;
	CvMemStorage* mem = cvCreateMemStorage(0);
	int ret;
	
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
/* boxArea filter - checks that the area of a contour
 * is at least minAreaRatio of that of the contour bounding box*/
int Contours::boxAreaFilter(double minAreaRatio){
	CvBox2D box;
	CvMemStorage* mem = cvCreateMemStorage(0);
	double boxArea,contourArea,dif,ret;
	
	box=cvMinAreaRect2(this->c,mem);
	boxArea=box.size.width*box.size.height;
	contourArea=this->getArea();
	dif=boxArea-contourArea;
	ret=(1- (dif/boxArea));
	
	/* Use this to set it by zone
	int zone=getPointZone(this->x,this->y);
	double minAreaRatioByZone[]={0,0.6,0.55,0.55,0.55};
    return ret > minAreaRatioByZone[zone];
    */
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
/*int Contours::histogramMatchingFilter(IplImage * src, CvHistogram * testImageHistogram,int h_bins,int s_bins, double min){
	CvRect box;
	CvMemStorage* mem = cvCreateMemStorage(0);
	
	double val;
	
	
	//get contour bounding box
	box=cvBoundingRect(this->c,0);
		
	IplImage * src_bbox=cvCreateImage(cvSize(box.width,box.height),src->depth,src->nChannels);
	
	//gets subimage bounded by box (Could change this to imageROI)
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
*/
std::vector<int> Contours::getCentroid(){
	 std::vector<int> vec(2);
	
	 CvMoments * myMoments = (CvMoments*)malloc( sizeof(CvMoments) );	 
	 cvMoments( this->c, myMoments );
	 vec[0]=(int) myMoments->m10/myMoments->m00;
	 vec[1]=(int) myMoments->m01/myMoments->m00;
	 
	 free(myMoments);
	 return vec;
}

/* returns sequence of points of the contour*/
CvSeq * Contours::getContour(){
	return this->c;
}

/* returns Circularity of the contour*/
double Contours::getCircularity(){
	double area=this->getArea();
	double per=this->getPerimeter();
	
	return (per*per)/area;
}

/*retuns Area of the contour*/
double Contours::getArea(){
	if(this->area==-1){
		this->area=fabs(cvContourArea(this->c,CV_WHOLE_SEQ));
	}

	return this->area;
		
}

/*retuns Perimeter of the contour*/	
double Contours::getPerimeter(){
	if(this->per==-1){
		this->per=cvContourPerimeter(this->c);
	}
	
	return this->per;
	
}
/*Filter by the number of points that conform the polygon */
int Contours::numberOfPointsFilter(int minNumberOfPoints=0){
	return this->c->total >= POLYGON_MINPOINTS;
	
}
/*
 * VasoFilter combines some of the other pre-defined filters to
 * determine if a given contour belongs to a plastic glass
 */
int Contours::vasoFilter(){
	
	CvSeq * points=this->c;
	CvPoint *longestEdge,*sndLongestEdge,*longestHead,*longestTail;
	double longestEdgeLen;
	int shorterEdgesCount=0;
	
	if(points->total<4)
		return false;
	
	//get longest edge
	longestEdge=getLongestEdge(points);
	//second longest edge
	sndLongestEdge=getLongestEdge2(points,&longestEdge[0],&longestEdge[1],&shorterEdgesCount);
	
	
	//longest edges should not share a same point
	if( equalsCvPoint(&longestEdge[0],&sndLongestEdge[0]) || 
		equalsCvPoint(&longestEdge[0],&sndLongestEdge[1]) ||
		equalsCvPoint(&longestEdge[1],&sndLongestEdge[1]) ||
		equalsCvPoint(&longestEdge[1],&sndLongestEdge[0]))
			return false;
			
	
	//checks that the two edges are separated from each other
	if(!minSeparationBetweenLongestEdges(longestEdge[0],longestEdge[1],sndLongestEdge[0],sndLongestEdge[1])){
		free(longestEdge);
		free(sndLongestEdge);
		return false;
	}
		
	
	double l1=edgeLength(longestEdge[0],longestEdge[1]);
	double l2=edgeLength(sndLongestEdge[0],sndLongestEdge[1]);
	
	free(longestEdge);
	free(sndLongestEdge);
	
	//~ printf(" vaso l2/l1 \n");
	
	//length of the two longest edges should be similar
	if(l2/l1 < VASO_LONGEST_EDGE_SIMILARITY)
		return false;
	
	//~ printf("eccentricity \n");
	
	double ecc=this->getCircularity();
	if(ecc<14 || ecc >18)
		return false;
	
	//~ printf("encontro vaso \n");
	
	return true;
}

/*
 * PlatoFilter combines some of the other pre-defined filters to
 * determine if a given contour belongs to a plastic Dish
 */
int Contours::platoFilter(){
	
	CvMemStorage* mem = cvCreateMemStorage(0);
	
	//Number of points should be at least 6.
	if(this->c->total<6)
		return false;
	
	CvBox2D box=cvMinAreaRect2(this->c,mem);
	CvBox2D box2=cvFitEllipse2(this->c);
	
	cvReleaseMemStorage(&mem);
	
	double majorAxis,minorAxis;
	double majorAxis2,minorAxis2;
	
	
	if(box2.size.width>box2.size.height){
		majorAxis=box2.size.width;
		minorAxis=box2.size.height;
	}else{
		minorAxis=box2.size.width;
		majorAxis=box2.size.height;
	}
	
	if(box.size.width>box.size.height){
		majorAxis2=box.size.width;
		minorAxis2=box.size.height;
	}else{
		minorAxis2=box.size.width;
		majorAxis2=box.size.height;
	}
	
	double boxDiff=fabs(majorAxis - majorAxis2) + fabs(minorAxis-minorAxis2);
	//eccentricity formula
	double ecc=sqrt(majorAxis*majorAxis -minorAxis*minorAxis)/majorAxis;
	
	
	double calcArea=miPI*(majorAxis/2)*(minorAxis/2);
	double realArea=this->getArea();
	
	//Area of approximated elipse and of the contour should be similar.
	if(fabs(calcArea-realArea)/realArea > 0.2)
		return false;
	
	//gets circularity
	double circ=this->getCircularity();
	
	if(circ<10 || circ>20)
		return false;
	
	if(ecc<0.65 || ecc>0.95)
		return false;
	
	return true;
}


}

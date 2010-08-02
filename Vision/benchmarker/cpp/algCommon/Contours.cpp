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


#define PER_TOLERANCE 50
#define POLYGON_MINPOINTS 4


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

bool equalsCvPoint(CvPoint * a, CvPoint * b){
	if(a==NULL || b==NULL)
		return false;
	return (a->x==b->x && a->y==b->y);
}
bool equalsEdge(CvPoint * head, CvPoint * tail,CvPoint * head2, CvPoint * tail2){
	if(head==NULL || tail==NULL || head2==NULL || tail2==NULL)
		return false;
		
	return (equalsCvPoint(head,head2) && equalsCvPoint(tail,tail2) ) ||
		((equalsCvPoint(head,tail2) && equalsCvPoint(tail,head2)))  ;
}
bool isIncreasing(CvPoint * tail,CvPoint * head){
	if( (tail->x < head->x && tail->y < head->y) || (tail->x > head->x && tail->y > head->y))
		return true;
		
	return false;
}
CvPoint * getLongestEdge(CvSeq * points,CvPoint * excludedHead=NULL,CvPoint * excludedTail=NULL){
	CvPoint * tail,*head,*tailMax,*headMax;
	int i;
	double delta,maxDelta;
	
	tailMax=NULL;
	headMax=NULL;
	//initial
	tail=CV_GET_SEQ_ELEM(CvPoint ,points,0);
	maxDelta=0;
		
	//traverse edges
	for(i=1; i< points->total;i++){
		head=CV_GET_SEQ_ELEM(CvPoint ,points,i);
		//ignore certain edges if told so
		if(!equalsEdge(head,tail,excludedHead,excludedTail))
		{			
			delta=edgeLength(*head,*tail);
			if(delta>maxDelta){
				tailMax=tail;
				headMax=head;
				maxDelta=delta;
			}
		}
		tail=head;
	}	
		
	//last edge
	head=CV_GET_SEQ_ELEM(CvPoint ,points,0);
	tail=CV_GET_SEQ_ELEM(CvPoint ,points,points->total -1);
	if(!equalsEdge(head,tail,excludedHead,excludedTail))
	{
		delta=edgeLength(*head,*tail);
		if(delta>maxDelta){
			tailMax=tail;
			headMax=head;
			maxDelta=delta;
		}
	}
	
	//result
	CvPoint * resp;
	if( (resp=(CvPoint*) malloc(2*sizeof(CvPoint)))==NULL){
		return NULL;
	}
	
	if((tailMax==NULL || headMax ==NULL)){
		return NULL;
	}
	
	
	resp[0]=*tailMax;
	resp[1]=*headMax;

	return resp;
}
CvPoint * getLongestEdge2(CvSeq * points,CvPoint * excludedHead=NULL,CvPoint * excludedTail=NULL,int * shorterEdges=NULL){
	CvPoint * tail,*head,*tailMax,*headMax;
	int i,shorterEdgeCounter;
	double delta,maxDelta,largestEdgeLen;
	
	
	shorterEdgeCounter=0;
	
	if(excludedHead!=NULL && excludedTail!=NULL)
		largestEdgeLen=edgeLength(*excludedHead,*excludedTail);
	else 
		largestEdgeLen=-1;
	
	tailMax=NULL;
	headMax=NULL;
	//initial
	tail=CV_GET_SEQ_ELEM(CvPoint ,points,0);
	maxDelta=0;
		
	//traverse edges
	for(i=1; i< points->total;i++){
		head=CV_GET_SEQ_ELEM(CvPoint ,points,i);
		//ignore certain edges if told so
		if(!equalsEdge(head,tail,excludedHead,excludedTail))
		{			
			delta=edgeLength(*head,*tail);
			if(delta>maxDelta){
				tailMax=tail;
				headMax=head;
				maxDelta=delta;
			}
			if(delta/largestEdgeLen <= 0.8)
				shorterEdgeCounter++;
		}
		tail=head;
	}	
		
	//last edge
	head=CV_GET_SEQ_ELEM(CvPoint ,points,0);
	tail=CV_GET_SEQ_ELEM(CvPoint ,points,points->total -1);
	if(!equalsEdge(head,tail,excludedHead,excludedTail))
	{
		delta=edgeLength(*head,*tail);
		if(delta>maxDelta){
			tailMax=tail;
			headMax=head;
			maxDelta=delta;
		}
		if(delta/largestEdgeLen <= 0.8)
			shorterEdgeCounter++;
	}
	
	//result
	CvPoint * resp;
	if( (resp=(CvPoint*) malloc(2*sizeof(CvPoint)))==NULL){
		return NULL;
	}
	
	if((tailMax==NULL || headMax ==NULL)){
		return NULL;
	}
	
	
	resp[0]=*tailMax;
	resp[1]=*headMax;
	
	if(shorterEdges!=NULL)
		*shorterEdges=shorterEdgeCounter;

	return resp;
}

double edgeLength(CvPoint head,CvPoint tail){
	double xdelta=fabs(head.x - tail.x);
	double ydelta=fabs(head.y - tail.y);
	return sqrt( xdelta*xdelta +ydelta*ydelta);
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


//test
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
	//~ return area>1000 ;
	return area>min_area && area<max_area;
	
}

int Contours::perimeterFilter(double min_per,double max_per){
	
	double per;
	per=this->getPerimeter();
		
	//double  maxPerimeterByZone[]={0,300,300,75,50};
	double  minPerimeterByZone[]={0,50,15,10,10};
	double  maxPerimeterByZone[]={0,300,200,100,50};
	
	int zone=getPointZone(this->x,this->y);
	
	//~ return per>100;// && per<10000;
	//~ return per>minPerimeterByZone[zone] &&  per< maxPerimeterByZone[zone];
	
	return per>min_per && per<max_per;
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
	//~ printf("dif %g \n",dif);
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

double Contours::getEccentricity(){
	double area=this->getArea();
	double per=this->getPerimeter();
	
	return (per*per)/area;
}

double Contours::getArea(){
	return fabs(cvContourArea(this->c,CV_WHOLE_SEQ));
}
		
double Contours::getPerimeter(){
	return cvContourPerimeter(this->c);
	
}
int Contours::numberOfPointsFilter(int minNumberOfPoints=0){
	return this->c->total >= POLYGON_MINPOINTS;
	
}

bool minSeparationBetweenLongestEdges(CvPoint head,CvPoint tail,CvPoint head2,CvPoint tail2){
	double a;
	double b;
	double len=edgeLength(head2,tail2);
	
	a=edgeLength(tail,tail2);
	b=edgeLength(tail,head2);
	if(a<b){
		//tail con tail , head con head
		//the edge connecting longest edges should be at least 30% of 2nd largest edge
		if(a < 0.3 * len)
			return false;
	}else
		if(b<0.3*len)
			return false;
			
	return true;
		
}


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
			
		
	if(!minSeparationBetweenLongestEdges(longestEdge[0],longestEdge[1],sndLongestEdge[0],sndLongestEdge[1])){
		free(longestEdge);
		free(sndLongestEdge);
		return false;
	}
		
	
	double l1=edgeLength(longestEdge[0],longestEdge[1]);
	double l2=edgeLength(sndLongestEdge[0],sndLongestEdge[1]);
	
	free(longestEdge);
	free(sndLongestEdge);
	
	
	//length of the two longest edges should be similar
	if(l2/l1 < 0.8)
		return false;
	
	double ecc=this->getEccentricity();
	if(ecc<14 || ecc >18)
		return false;
	
	
	
	return true;
}

int Contours::platoFilter(){
	
	CvMemStorage* mem = cvCreateMemStorage(0);
	
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
	
	
	//~ if(ecc>0.95)
		//~ return false;
	//estimated area
	
	double calcArea=PI*(majorAxis/2)*(minorAxis/2);
	double realArea=this->getArea();
	
	if(fabs(calcArea-realArea)/realArea > 0.2)
		return false;
	
	double circ=this->getEccentricity();
	
	if(circ<10 || circ>20)
		return false;
	
	if(ecc<0.65 || ecc>0.95)
		return false;
	
	return true;
}


}

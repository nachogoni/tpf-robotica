#include "GarbageRecognition.h"
#include "Prediction.h"

//~ #define BENCHMARK_H
#include <highgui.h>
#include <cv.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <time.h>
#include "Contours.h"
#include "Garbage.h"
#include "Windowing.h"
#include "MinimalBoundingRectangle.h"
#include "PredictionParameters.h"

// image preprocessing values
#define THRESHOLD_VALUE 240
#define MORPH_KERNEL_SIZE 1
#define MORPH_DILATE_ITER 2
#define MORPH_ERODE_ITER 1

#define _RED  cvScalar (0, 0, 255, 0)
#define _GREEN cvScalar (0, 255, 0, 0)

//contour filters constants
#define MINCONTOUR_AREA 1000
#define MAXCONTOUR_AREA 10000
#define BOXFILTER_TOLERANCE 0.7
#define MINCONTOUR_PERIMETER 2
#define MAXCONTOUR_PERIMETER 1000
#define CONTOUR_RECTANGULAR_MIN_RATIO 1.2
#define CONTOUR_RECTANGULAR_MAX_RATIO 3.0
#define HIST_S_BINS 8 
#define HIST_H_BINS 8
#define HIST_MIN 0.7
#define TIME_THRESHOLD 15 //seconds

#define ENABLE_WINDOWING true

#ifdef BENCHMARK_H
	extern int focusedFrames;
#endif

namespace utils {

void drawPrediction(IplImage * src,std::list<utils::Garbage*> garbagePrediction);


std::list<utils::Garbage*> garbages;
std::list<utils::Garbage*> garbagePrediction;

IplImage * src_window;


GarbageRecognition::GarbageRecognition(WorldInfo * wi=0){
	
	if(wi!=NULL)
		this->wi=wi;
		
	this->prediction= new Prediction();
	this->window=NULL;
	frameNumber=0;
	focused=false;
	doWindowing=true;
	doPrediction=true;
	}
	
GarbageRecognition::~GarbageRecognition(){};
	
std::list<Garbage*> 
GarbageRecognition::getGarbageList()
{
	IplImage * src = this->loadImage();
		//~ IplImage * model = cvLoadImage("./colilla-sinBlanco.png",1);
		IplImage * copy;
		//windowing
		if(this->focused){
			if(window->currentGarbage->state!=DEAD){
				if(src_window!=NULL){
					if(window->last_last_window!=NULL)
						//cvReleaseImageHeader(&(window->last_last_window));
				
					//logic to release sub-windowing memory
					if(window->release_window){
						window->last_last_window=window->last_window;
						window->last_window=src_window;
					}
				
				}
				
				src_window=this->window->getWindow(src);
				if(src_window==NULL){
					this->focused=false;
					delete window;
					src_window=src;
				}
				window->release_window=true;
			}
			else{
				this->focused=false;
				delete window;
				src_window=src;
			}
		}else{
			src_window=src;
		}
		
		//get garbages from vision system
		garbages = this->garbageList(src_window,NULL);
		
		if(this->focused){
			garbages=this->window->correctGarbages(garbages);
			#ifdef BENCHMARK_H
				focusedFrames++;
			#endif
		}
		
		//Feed retrieved garbages to prediction system
		if(this->doPrediction)
			garbagePrediction= this->prediction->getPrediction(garbages);
		else
			garbagePrediction=garbages;
		
		//start  windowing
		if(this->doWindowing && (this->frameNumber % NUMBER_OF_FRAMES_TO_FOCUS) && this->focused==false ){
			
			GarbageHistoric * focusedGarbage=prediction->focusGarbage();
			if(focusedGarbage!=NULL){
				this->window=new Windowing(focusedGarbage,cvGetSize(src));
				this->focused=true;
				window->release_window=false;
			}
		}
		
		drawPrediction(src,garbagePrediction);
		
		//~ cvReleaseImage(&model);
		this->frameNumber++;
	cvReleaseImage(&src);
	//return garbages;
	return garbagePrediction;
}

std::list<Garbage*> 
GarbageRecognition::garbageList(IplImage * src, IplImage * model){

    clock_t start = clock();

                    
	std::list<Garbage*> garbageList;
	std::vector<int> centroid(2);
	
	//~ utils::Histogram * h = new Histogram(HIST_H_BINS,HIST_S_BINS);
	//~ CvHistogram * testImageHistogram = h->getHShistogramFromRGB(model);

	//gets a frame for setting  image size
	CvSize srcSize = cvGetSize(src);
	CvRect srcRect = cvRect(0,0,srcSize.width,srcSize.height);
	
	//images for HSV conversion
	IplImage* hsv = cvCreateImage( srcSize, 8, 3 );
	IplImage* h_plane = cvCreateImage( srcSize, 8, 1 );
	IplImage* h_plane2 = cvCreateImage( srcSize, 8, 1 );
	IplImage* h_planeV ;//= cvCreateImage( srcSize, 8, 1 );
	IplImage* s_plane = cvCreateImage( srcSize, 8, 1 );
	IplImage* v_plane = cvCreateImage( srcSize, 8, 1 );

	//Image for filtering
	IplImage * andImage=cvCreateImage(srcSize,8,1);	
	IplImage * andImageV=cvCreateImage(srcSize,8,1);	
	//Image for thresholding
	IplImage * threshImage=cvCreateImage(srcSize,8,1);
	IplImage * threshImageV=cvCreateImage(srcSize,8,1);

	//image for Morphing operations(Dilate-erode)
	IplImage * morphImage=cvCreateImage(srcSize,8,1);
	IplImage * morphImageV=cvCreateImage(srcSize,8,1);
	
	//image for contour-finding operations
	IplImage * contourImage=cvCreateImage(srcSize,8,3);
	
	clock_t  create=clock();
	printf("Time elapsed create Image: %f\n", ((double)create - start) / CLOCKS_PER_SEC);
	int frameCounter=1;
	int cont_index=0;

	//convolution kernel for morph operations
	IplConvKernel* element;

	CvRect boundingRect;

	//contours
	CvSeq * contours;
	CvSeq * contoursCopy;

	//Main loop


	
	//convert image to hsv
	cvCvtColor( src, hsv, CV_BGR2HSV );
	clock_t  conv=clock();
	printf("Time elapsed create Image- convert image: %f\n", ((double)conv - create) / CLOCKS_PER_SEC);
	
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
	h_planeV=cvCloneImage(h_plane);
	h_plane2=cvCloneImage(h_plane);
	
	CvScalar vasosL1 = cvScalar (0, 0, 170);
	CvScalar vasosU1 = cvScalar (20, 255, 255);
	CvScalar vasosL = cvScalar (40, 0, 170);
	CvScalar vasosU = cvScalar (255, 255, 255);
	CvScalar colillasL = cvScalar (20, 60, 0);
	CvScalar colillasU = cvScalar (40, 255,255);

	clock_t  inrange=clock();
	//~ cvInRangeSalt( hsv,vasosL,vasosU, vasosL1, vasosU1,h_plane );
	cvInRangeS( hsv, vasosL1, vasosU1, h_plane );
	cvInRangeS( hsv, vasosL, vasosU, h_plane2 );
	cvOr(h_plane,h_plane2,h_plane);
	printf("inRange  %f\n", ((double)clock() - inrange) / CLOCKS_PER_SEC);

	cvInRangeS( hsv, colillasL,colillasU,h_planeV);
	cvShowImage("inrange vasos",h_plane);
	//~ cvShowImage("inrange colillas",h_planeV);
	//~ for(int x=0;x<srcSize.width;x++){
		//~ for(int y=0;y<srcSize.height;y++){
			//~ uchar * hue=&((uchar*) (h_plane->imageData+h_plane->widthStep*y))[x];
			//~ uchar * hueV=&((uchar*) (h_planeV->imageData+h_plane->widthStep*y))[x];
			//~ uchar * sat=&((uchar*) (s_plane->imageData+s_plane->widthStep*y))[x];
			//~ uchar * val=&((uchar*) (v_plane->imageData+v_plane->widthStep*y))[x];
			
			//~ if((*val>170) && (( (*hue)<20 || (*hue)>40) ))
				//~ *hue=255;
			//~ else
				//~ *hue=0;
			
			//filter for cigar filters
			//~ if((*hueV>20 && *hueV<40 && *sat>60))
				//~ *hueV=255;
			//~ else
				//~ *hueV=0;
		//~ }
	//~ }
	
	clock_t  color=clock();
	printf("Time elapsed create Image - color filter: %f\n", ((double)color - conv) / CLOCKS_PER_SEC);
	
	//--first pipeline
	//apply morphologic operations
	element = cvCreateStructuringElementEx( MORPH_KERNEL_SIZE*2+1,
		MORPH_KERNEL_SIZE*2+1, MORPH_KERNEL_SIZE, MORPH_KERNEL_SIZE,
		CV_SHAPE_RECT, NULL);

	cvDilate(h_plane,morphImage,element,MORPH_DILATE_ITER);
	cvErode(morphImage,morphImage,element,MORPH_ERODE_ITER);
	
	cvThreshold(morphImage,threshImage,100,255,CV_THRESH_BINARY);
	
	clock_t  pipe1=clock();
	printf("Time elapsed color filter - first pipeline: %f\n", ((double)pipe1 - color) / CLOCKS_PER_SEC);
	
	//-- end first pipeline
	//-- start 2nd pipeline-----
	
	cvAnd(h_planeV, v_plane, andImageV);
	//apply morphologic operations

	cvDilate(andImageV,morphImageV,element,MORPH_DILATE_ITER);
	cvErode(morphImageV,morphImageV,element,MORPH_ERODE_ITER);
	
	cvThreshold(morphImageV,threshImageV,100,255,CV_THRESH_BINARY);

    //--end second pipeline--
    clock_t  pipe2=clock();
	printf("Time elapsed first pipeline - second pipeline: %f\n", ((double)pipe2 - pipe1) / CLOCKS_PER_SEC);
	//get all contours
	contours=myFindContours(threshImage);
	contoursCopy=contours;
	
	cont_index=0;
	
	//image to write contours on
	cvCopy(src,contourImage,0);
	
	
	//contours for dishes and glasses
	while(contours!=NULL){

		CvSeq * aContour=getPolygon(contours);
		utils::Contours * ct;
		
		if(this->window==NULL)
			ct = new Contours(aContour);
		else
			ct = new Contours(aContour,this->window->window);
		
		//apply filters for vasos

		if( ct->perimeterFilter(100,10000) && 
			ct->areaFilter(1000,100000) &&
			ct->vasoFilter()
			){	
				//get contour bounding box
				boundingRect=cvBoundingRect(ct->getContour(),0);
				cvRectangle(contourImage,cvPoint(boundingRect.x,boundingRect.y),
						cvPoint(boundingRect.x+boundingRect.width,
						boundingRect.y+boundingRect.height),
						_GREEN,1,8,0);
						
				
				//if passed filters
				ct->printContour(3,cvScalar(127,127,0,0),
					contourImage);
								
				centroid=ct->getCentroid();
				
				//build garbage List
				utils::MinimalBoundingRectangle * r = new utils::MinimalBoundingRectangle(boundingRect.x,
					boundingRect.y,boundingRect.width,boundingRect.height);

				utils::Garbage * aGarbage = new utils::Garbage(r,centroid,ct);
				//benchmark purposes
				aGarbage->isVisualized=true;
				aGarbage->isPredicted=false;
				aGarbage->isFocused=false;
				

				garbageList.push_back(aGarbage);
			}else if( ct->perimeterFilter(100,10000) && 
			ct->areaFilter(1000,100000) &&
			ct->platoFilter()
			){	
				//get contour bounding box
				boundingRect=cvBoundingRect(ct->getContour(),0);
				cvRectangle(contourImage,cvPoint(boundingRect.x,boundingRect.y),
						cvPoint(boundingRect.x+boundingRect.width,
						boundingRect.y+boundingRect.height),
						_GREEN,1,8,0);
							
				//if passed filters
				ct->printContour(3,cvScalar(127,127,0,0),
					contourImage);
								
				centroid=ct->getCentroid();
				
				//build garbage List
				utils::MinimalBoundingRectangle * r = new utils::MinimalBoundingRectangle(boundingRect.x,
					boundingRect.y,boundingRect.width,boundingRect.height);

				utils::Garbage * aGarbage = new utils::Garbage(r,centroid,ct);
				//benchmark purposes
				aGarbage->isVisualized=true;
				aGarbage->isPredicted=false;
				aGarbage->isFocused=false;
				

				garbageList.push_back(aGarbage);
			}

		//delete ct;
		cvReleaseMemStorage( &aContour->storage );
		contours=contours->h_next;
		cont_index++;
	}
	clock_t  vasoyplato=clock();
	printf("Time elapsed first pipe2 - vasos y platos: %f\n", ((double)vasoyplato - pipe2) / CLOCKS_PER_SEC);
	
		//2nd pipeline
		//release temp images and data
		if(contoursCopy!=NULL)
			cvReleaseMemStorage( &contoursCopy->storage );
		
		contours=myFindContours(threshImageV);
		contoursCopy=contours;
		cont_index=0;

		
		while(contours!=NULL){

		CvSeq * aContour=getPolygon(contours);
		utils::Contours * ct;
		
		if(this->window==NULL)
			ct = new Contours(aContour);
		else
			ct = new Contours(aContour,this->window->window);
		
		//apply filters

    
		if( ct->perimeterFilter(10,800) && 
			ct->areaFilter(50,800) &&
			//ct->rectangularAspectFilter(CONTOUR_RECTANGULAR_MIN_RATIO, CONTOUR_RECTANGULAR_MAX_RATIO) && 
			ct->boxAreaFilter(BOXFILTER_TOLERANCE) && 	
			//ct->histogramMatchingFilter(src,testImageHistogram, HIST_H_BINS,HIST_S_BINS,HIST_MIN)&&
			1){	
				//get contour bounding box
				boundingRect=cvBoundingRect(ct->getContour(),0);
				cvRectangle(contourImage,cvPoint(boundingRect.x,boundingRect.y),
						cvPoint(boundingRect.x+boundingRect.width,
						boundingRect.y+boundingRect.height),
						_GREEN,1,8,0);
										
				//if passed filters
				ct->printContour(3,cvScalar(127,127,0,0),
					contourImage);
								
				centroid=ct->getCentroid();
				
				//build garbage List
				utils::MinimalBoundingRectangle * r = new utils::MinimalBoundingRectangle(boundingRect.x,
					boundingRect.y,boundingRect.width,boundingRect.height);

				utils::Garbage * aGarbage = new utils::Garbage(r,centroid,ct);
				//benchmark purposes
				aGarbage->isVisualized=true;
				aGarbage->isPredicted=false;
				aGarbage->isFocused=false;

				garbageList.push_back(aGarbage);
			}

		delete ct;
		cvReleaseMemStorage( &aContour->storage );
		contours=contours->h_next;
		cont_index++;
	}
	clock_t  colillas=clock();
	printf("Time elapsed vasosyplatos - colillas: %f\n", ((double)colillas - vasoyplato) / CLOCKS_PER_SEC);

	//display found contours
    //~ cvShowImage("drawContours",contourImage);
	
	
	//release temp images and data
	if(contoursCopy!=NULL)
		cvReleaseMemStorage( &contoursCopy->storage );
	
	
	cvReleaseStructuringElement(&element);
	cvReleaseImage(&threshImage);
	cvReleaseImage(&threshImageV);
	cvReleaseImage(&morphImage);
	cvReleaseImage(&morphImageV);
	cvReleaseImage(&contourImage);
	cvReleaseImage(&hsv);
	cvReleaseImage(&h_plane);
	cvReleaseImage(&h_planeV);
	cvReleaseImage(&s_plane);
	cvReleaseImage(&v_plane);
	cvReleaseImage(&andImageV);
	cvReleaseImage(&andImage);
	
	clock_t  total=clock();
	printf("total: %f\n", ((double)total - start) / CLOCKS_PER_SEC);
	
	return garbageList;
}

void drawPrediction(IplImage * src,std::list<Garbage*> garbagePrediction){
	CvSize srcSize = cvGetSize(src);
	IplImage * predictionImage=cvCreateImage(srcSize,8,3);
	
	cvCopy(src,predictionImage,0);	
	MinimalBoundingRectangle * boundingRect;
	
	for (std::list<Garbage*>::iterator itGar = garbagePrediction.begin(); itGar != garbagePrediction.end(); itGar++)
	{
		boundingRect=(*itGar)->boundingBox();
		cvRectangle(predictionImage,cvPoint(boundingRect->x,boundingRect->y),
								cvPoint(boundingRect->x+boundingRect->width,
								boundingRect->y+boundingRect->height),
								_RED,1,8,0);
	}
	
	cvShowImage("prediction",predictionImage);
	
	cvReleaseImage(&predictionImage);
}

	void GarbageRecognition::enablePrediction(){
		this->doPrediction=true;
	}
	
    void GarbageRecognition::disablePrediction(){
		this->doPrediction=false;
	}
    void GarbageRecognition::enableWindowing(){
		this->doWindowing=true;
	}
    void GarbageRecognition::disableWindowing(){
		this->doWindowing=false;
	}

} /* End of namespace utils */

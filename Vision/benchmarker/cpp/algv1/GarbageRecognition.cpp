#include "GarbageRecognition.h"
#include "Prediction.h"

#include <highgui.h>
#include <cv.h>
#include <vector>
#include <stdio.h>
#include <string>
#include "Contours.h"
#include "Histogram.h"
#include "Garbage.h"
#include "Windowing.h"
#include "MinimalBoundingRectangle.h"

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
#define BOXFILTER_TOLERANCE 0.55
#define MINCONTOUR_PERIMETER 80
#define MAXCONTOUR_PERIMETER 1000
#define CONTOUR_RECTANGULAR_MIN_RATIO 1.2
#define CONTOUR_RECTANGULAR_MAX_RATIO 3.0
#define HIST_S_BINS 8
#define HIST_H_BINS 8
#define HIST_MIN 0.7
#define TIME_THRESHOLD 15 //seconds




/*
int main(void)
{
	utils::GarbageRecognition * gr= new utils::GarbageRecognition();
	
	IplImage * src = cvLoadImage("./colilla-scene.png",1);
	gr->getGarbageList(src);
	
}
*/
namespace utils {

void drawPrediction(IplImage * src,std::list<utils::Garbage*> garbagePrediction);


std::list<utils::Garbage*> garbages;
std::list<utils::Garbage*> garbagePrediction;


GarbageRecognition::GarbageRecognition(){
	this->prediction= new Prediction();
	this->window=new Windowing();
	frameNumber=0;
	focused=false;
	
	}
GarbageRecognition::~GarbageRecognition(){};
	
std::list<Garbage*> 
GarbageRecognition::getGarbageList(IplImage * src)
{
		IplImage * model = cvLoadImage("./colilla-sinBlanco.png",1);
		IplImage * src_window;
		//windowing
		if(this->focused){
			if(window->currentGarbage->state!=DEAD){
				src_window=this->window->getWindow(src);
				if(src_window==NULL){
					this->focused=false;
					delete window;
					src_window=src;
				}
			}
			else{
				this->focused=false;
				delete window;
				src_window=src;
			}
		}else{
			src_window=src;
		}
		
		garbages = this->garbageList(src_window,model);
		
		if(this->focused){
			garbages=this->window->correctGarbages(garbages);
		}
		
		//prediction
		garbagePrediction= this->prediction->getPrediction(garbages);
		
		//start  windowing
		if(!(this->frameNumber % 20) && this->focused==false){
			GarbageHistoric * focusedGarbage=prediction->focusGarbage();
			if(focusedGarbage!=NULL){
				this->window=new Windowing(focusedGarbage,cvGetSize(src));
				this->focused=true;
			}
		}
		
		printf(" Garbage Prediction %d\n",garbagePrediction.size());
		drawPrediction(src,garbagePrediction);
		
		cvReleaseImage(&model);
		this->frameNumber++;
	//return garbages;
	return garbagePrediction;
}

std::list<Garbage*> 
GarbageRecognition::garbageList(IplImage * src, IplImage * model){

                    
    
	std::list<Garbage*> garbageList;

	//cvNamedWindow("output",CV_WINDOW_AUTOSIZE);
	//object model

	//image for the histogram-based filter
	//could be a parameter
	
	//~ cvNamedWindow("andImage",CV_WINDOW_AUTOSIZE);
	//~ cvNamedWindow("andSimage",CV_WINDOW_AUTOSIZE);
	//~ cvNamedWindow("andSIImage",CV_WINDOW_AUTOSIZE);
	//~ cvNamedWindow("drawContours",CV_WINDOW_AUTOSIZE);
	//~ cvNamedWindow("andSThreshImage",CV_WINDOW_AUTOSIZE);
	//~ cvNamedWindow("threshImage",CV_WINDOW_AUTOSIZE);
//	cvNamedWindow("andSequalizedImage",CV_WINDOW_AUTOSIZE);
	//~ cvNamedWindow("morphImage",CV_WINDOW_AUTOSIZE);

	utils::Histogram * h = new Histogram(HIST_H_BINS,HIST_S_BINS);
	CvHistogram * testImageHistogram = h->getHShistogramFromRGB(model);

	//~ int frameWidth=cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH);
	//~ int frameHeight=cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT);



	//gets a frame for setting  image size
	//CvSize srcSize = cvSize(frameWidth,frameHeight);
	CvSize srcSize = cvGetSize(src);
	

	//images for HSV conversion
	IplImage* hsv = cvCreateImage( srcSize, 8, 3 );
	IplImage* h_plane = cvCreateImage( srcSize, 8, 1 );
	IplImage* s_plane = cvCreateImage( srcSize, 8, 1 );
	IplImage* v_plane = cvCreateImage( srcSize, 8, 1 );


	//Image for thresholding
	IplImage * andImage=cvCreateImage(srcSize,8,1);
	
	IplImage * andSimage=cvCreateImage(srcSize,8,1);
	IplImage * andSThreshImage=cvCreateImage(srcSize,8,1);
	IplImage * andSequalizedImage=cvCreateImage(srcSize,8,1);
	IplImage * andSIImage=cvCreateImage(srcSize,8,1);

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

	//Main loop


	//convert image to hsv
	cvCvtColor( src, hsv, CV_BGR2HSV );
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
	
	
	/*I(x,y)blue ~ ((uchar*)(img->imageData + img->widthStep*y))[x*3]
	I(x,y)green ~ ((uchar*)(img->imageData + img->widthStep*y))[x*3+1]
	I(x,y)red ~ ((uchar*)(img->imageData + img->widthStep*y))[x*3+2]*/
	
	for(int x=0;x<srcSize.width;x++){
		for(int y=0;y<srcSize.height;y++){
			uchar * hue=&((uchar*) (h_plane->imageData+h_plane->widthStep*y))[x];
			uchar * sat=&((uchar*) (s_plane->imageData+s_plane->widthStep*y))[x];
			uchar * val=&((uchar*) (v_plane->imageData+v_plane->widthStep*y))[x];
			if((*hue>20 && *hue<40 && *sat>60))
				*hue=255;
			else
				*hue=0;
		}
	}
	cvAnd(h_plane, v_plane, andImage);
	cvAnd(h_plane, s_plane, andSimage);
	
	
	//apply morphologic operations
	element = cvCreateStructuringElementEx( MORPH_KERNEL_SIZE*2+1,
		MORPH_KERNEL_SIZE*2+1, MORPH_KERNEL_SIZE, MORPH_KERNEL_SIZE,
		CV_SHAPE_RECT, NULL);

	
	cvDilate(andImage,morphImage,element,MORPH_DILATE_ITER);
	cvErode(morphImage,morphImage,element,MORPH_ERODE_ITER);
	
	
	cvThreshold(morphImage,threshImage,120,255,CV_THRESH_BINARY);
	
	//get all contours
	contours=myFindContours(threshImage);
	//contours=myFindContours(smoothImage);


	cont_index=0;
	
	cvCopy(src,contourImage,0);
	


	while(contours!=NULL){

		CvSeq * aContour=getPolygon(contours);
		utils::Contours * ct = new Contours(aContour);

	


		//apply filters

    
		if( ct->perimeterFilter(MINCONTOUR_PERIMETER,MAXCONTOUR_PERIMETER) && 
			ct->areaFilter(MINCONTOUR_AREA,MAXCONTOUR_AREA) &&
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
				
				std::vector<int> centroid(2);
				centroid=ct->getCentroid();

					
				//build garbage List
				utils::MinimalBoundingRectangle * r = new utils::MinimalBoundingRectangle(boundingRect.x,
					boundingRect.y,boundingRect.width,boundingRect.height);

				utils::Garbage * aGarbage = new utils::Garbage(r,centroid);

				garbageList.push_back(aGarbage);


			}

		delete ct;
		cvReleaseMemStorage( &aContour->storage );
		contours=contours->h_next;
		cont_index++;
	}

    cvShowImage("drawContours",contourImage);
   // cvWaitKey(0);
	delete h;
	
	
	cvReleaseHist(&testImageHistogram);
	//Image for thresholding
	//cvReleaseMemStorage( &contours->storage );
	cvReleaseImage(&threshImage);
	cvReleaseImage(&equalizedImage);
	cvReleaseImage(&morphImage);
	cvReleaseImage(&smoothImage);
	cvReleaseImage(&contourImage);
	
	cvReleaseImage(&hsv);
	cvReleaseImage(&h_plane);
	cvReleaseImage(&s_plane);
	cvReleaseImage(&v_plane);
	cvReleaseImage(&andImage);
	cvReleaseImage(&andSimage);
	cvReleaseImage(&andSThreshImage);
	cvReleaseImage(&andSequalizedImage);
	
	

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

} /* End of namespace utils */

#include <vision/GarbageRecognition.h>

#include <highgui.h>
#include <stdio.h>
#include <string>
#include <time.h>
#include <vision/Contours.h>
#include <vision/Histogram.h>
#include <vision/Garbage.h>
#include <utils/MyAngle.h>
#include <vision/MinimalBoundingRectangle.h>

// image preprocessing values
#define THRESHOLD_VALUE 240
#define MORPH_KERNEL_SIZE 2
#define MORPH_DILATE_ITER 0
#define MORPH_ERODE_ITER 0

#define _RED  cvScalar (0, 0, 255, 0)
#define _GREEN cvScalar (0, 255, 0, 0)

//contour filters constants
#define MINCONTOUR_AREA 500
#define MAXCONTOUR_AREA 10000
#define BOXFILTER_TOLERANCE 0.55
#define MINCONTOUR_PERIMETER 30
#define MAXCONTOUR_PERIMETER 300
#define CONTOUR_RECTANGULAR_MIN_RATIO 1.2
#define CONTOUR_RECTANGULAR_MAX_RATIO 3.0
#define HIST_S_BINS 8
#define HIST_H_BINS 8
#define HIST_MIN 0.7
#define TIME_THRESHOLD 1 //seconds

namespace vision {


GarbageRecognition::GarbageRecognition(WorldInfo * wi){
	this->wi = wi;
    this->model = cvLoadImage("./colilla-sinBlanco.png",1);
}

std::list<Garbage*> GarbageRecognition::getGarbageList()
{
	if ( ! this->pooled ){
		this->pooled = true;
	    IplImage * src = this->loadImage();
		this->garbageList(src,this->model);
		cvReleaseImage(&src);
	}
	return garbages;
}



std::list<vision::Garbage*> GarbageRecognition::garbageList(IplImage * src, IplImage * model){
	std::list<vision::Garbage*>::iterator it;
	for ( it=garbages.begin() ; it != garbages.end() ; it++ )
		delete *it;
	garbages.clear();
  

	//cvNamedWindow("output",CV_WINDOW_AUTOSIZE);
	//object model

	//image for the histogram-based filter
	//could be a parameter

	vision::Histogram * h = new Histogram(HIST_H_BINS,HIST_S_BINS);
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
  CvSeq * contoursCopy;

	//Main loop


	frameCounter++;

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
	contours = myFindContours(smoothImage);
  contoursCopy=contours;
	
  cont_index=0;
	cvCopy(src,contourImage,0);
	


	while(contours!=NULL){
		CvSeq * aContour=getPolygon(contours);
		vision::Contours * ct = new Contours(aContour);

	
	    //int	pf = ct->perimeterFilter(MINCONTOUR_PERIMETER,MAXCONTOUR_PERIMETER);

		//int raf = ct->rectangularAspectFilter(CONTOUR_RECTANGULAR_MIN_RATIO, CONTOUR_RECTANGULAR_MAX_RATIO);

		// int af = ct->areaFilter(MINCONTOUR_AREA,MAXCONTOUR_AREA);
		//int baf = ct->boxAreaFilter(BOXFILTER_TOLERANCE);

        //int hmf = ct->histogramMatchingFilter(src,testImageHistogram, HIST_H_BINS,HIST_S_BINS,HIST_MIN);


		//apply filters

		if (ct->perimeterFilter(MINCONTOUR_PERIMETER,MAXCONTOUR_PERIMETER) &&
        ct->rectangularAspectFilter(CONTOUR_RECTANGULAR_MIN_RATIO, CONTOUR_RECTANGULAR_MAX_RATIO) &&
        ct->boxAreaFilter(BOXFILTER_TOLERANCE) &&
        ct->histogramMatchingFilter(src,testImageHistogram, HIST_H_BINS,HIST_S_BINS,HIST_MIN) &&
       1){

				
				
				//get contour bounding box
				boundingRect=cvBoundingRect(ct->getContour(),0);
				
				//build garbage List
			
				//printf(" c %d,%d\n",boundingRect.x,boundingRect.y);

				vision::MinimalBoundingRectangle * r = new vision::MinimalBoundingRectangle(boundingRect.x,
					boundingRect.y,boundingRect.width,boundingRect.height);



				vision::Garbage * aGarbage = new vision::Garbage(r);
//				printf("%d , %d - %d , %d\n",boundingRect.x,boundingRect.y,boundingRect.width,boundingRect.height);

				garbages.push_back(aGarbage);


			}

		delete ct;
		cvReleaseMemStorage( &aContour->storage );
		contours=contours->h_next;
		cont_index++;
	}

   // cvShowImage("output",contourImage);
   // cvWaitKey(0);
	delete h;
  
  if(contoursCopy!=NULL)
    cvReleaseMemStorage(&contoursCopy->storage);

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

	return garbages;
}

} /* End of namespace vision */

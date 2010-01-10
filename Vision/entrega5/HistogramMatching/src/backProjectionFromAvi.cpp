	//      compareByBackProjection.cpp
//      
//      Copyright 2010 dnul <dnul@alu.itba.edu.ar>
//      


#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include "2dhist.c"

int main(int argc, char** argv)
{
	//windows
	cvNamedWindow("backProjection",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("EQbackProjection",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("src",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("test",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("backProjectionThreshold",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("testHistogram",CV_WINDOW_AUTOSIZE);
	
	//image to search
    IplImage* src;
    //test Image to find
    IplImage* test;
    
    //video to search
    CvCapture* capture;
    
    if( argc == 3 && (capture=cvCreateFileCapture( argv[1]))!= 0 && (test=cvLoadImage(argv[2], 1))!= 0){
	
		//get first frame
		src=cvQueryFrame(capture);
		//~ IplImage* downSampledImage=cvCreateImage(cvSize(src->width/2,src->height/2),src->depth,src->nChannels);
		//~ 
		//~ cvPyrDown( src, downSampledImage);
		//~ src=downSampledImage;
		
		
		//set Sizes
		CvSize backProjectionSize = cvSize(src->width - test->width +1,src->height - test->height +1);
		CvSize patchSize = cvGetSize(test);
		
		
		//Precision variables
		int hist_Hue_bins=8;
		int hist_Sat_bins=8;
		float histogramNormalizationFactor=1.0;		
		double threshBackProjectionValue=0.15;
		
		//output image
		IplImage * backProjection = cvCreateImage(backProjectionSize, 32, 1 );
		
		IplImage* testHistogramImage;
		//convert the image to HSV
		IplImage* h_plane = cvCreateImage( cvGetSize(src), 8, 1 );
		IplImage* s_plane = cvCreateImage( cvGetSize(src), 8, 1 );
		IplImage* v_plane = cvCreateImage( cvGetSize(src), 8, 1 );
		IplImage* planes[] = { h_plane, s_plane };
		IplImage* hsv = cvCreateImage( cvGetSize(src), 8, 3 );
		
		//create histogram of test(model) Image
		CvHistogram * testImageHistogram = getHShistogram(test,hist_Hue_bins,hist_Sat_bins);
		
		//Thresholded output image
		IplImage * threshImage=cvCreateImage(cvGetSize(backProjection),8,1);
		//aux images
		IplImage* eqBackProjection = cvCreateImage( cvGetSize(backProjection), 8, 1 );
        IplImage* backProjection_8b = cvCreateImage( cvGetSize(backProjection), 8, 1 );

		
		cvShowImage("test",test);
		
		//main Loop
		int frameNumber=0;
		while(src=cvQueryFrame(capture)){ 
			
			//~ cvPyrDown( src, downSampledImage);
			//~ src=downSampledImage;
			
			
			cvCvtColor( src, hsv, CV_BGR2HSV );
			cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
			
			//Leaves only the main values of the hist
			cvThreshHist(testImageHistogram,threshBackProjectionValue);
			//goes through the src image finding Histogram equivalence
			cvCalcBackProjectPatch( planes, backProjection,
                             patchSize, testImageHistogram,
                             CV_COMP_BHATTACHARYYA, histogramNormalizationFactor );
			
			//draws 2d Histogram
			//testHistogramImage=draw2dHistogram(testImageHistogram,hist_Hue_bins,hist_Sat_bins,10);
			
			//~ cvShowImage("testHistogram",testHistogramImage);
			
			//~ //Threshold backProjection Image
			//~ cvThreshold(backProjection,threshImage,threshBackProjectionValue,255,CV_THRESH_BINARY_INV);
			//~ 
			//~ cvConvertScale( backProjection, backProjection_8b, 1, 0) ;
        
			printf("frame number %d\n",frameNumber++);
			//show everything else
			cvShowImage("backProjection",backProjection);
			cvShowImage("src",src);
			
			cvWaitKey();
                             
		}
		
		
        
                       
        
        
		
	}else{
		printf("could not load input\n");
	}
	
	return 0;
}


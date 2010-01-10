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
	//image to search
    IplImage* src;
    //test Image to find
    IplImage* test;
    
    if( argc == 3 && (src=cvLoadImage(argv[1], 1))!= 0 && (test=cvLoadImage(argv[2], 1))!= 0){
		
		CvSize backProjectionSize = cvSize(src->width - test->width +1,src->height - test->height +1);
		CvSize patchSize = cvGetSize(test);
		
		//Precision variables
		int hist_Hue_bins=8;
		int hist_Sat_bins=8;
		float histogramNormalizationFactor=1.0;
		
		double threshBackProjectionValue=0.0;
		
		IplImage* testHistogramImage;
		//convert the image to HSV
		IplImage* h_plane = cvCreateImage( cvGetSize(src), 8, 1 );
		IplImage* s_plane = cvCreateImage( cvGetSize(src), 8, 1 );
		IplImage* v_plane = cvCreateImage( cvGetSize(src), 8, 1 );
		IplImage* planes[] = { h_plane, s_plane };
		IplImage* hsv = cvCreateImage( cvGetSize(src), 8, 3 );
		
		cvCvtColor( src, hsv, CV_BGR2HSV );
		cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
		
		//output image
		IplImage * backProjection = cvCreateImage(backProjectionSize, 32, 1 );
		
		//create histogram for test Image
		//~ CvHistogram * testImageHistogram = getHShistogram(test);
		CvHistogram * testImageHistogram = getHShistogram(test,hist_Hue_bins,hist_Sat_bins);
		//~ cvNormalizeHist(testImageHistogram,histogramNormalizationFactor);
		
		 
		//cvThreshHist(testImageHistogram,0.025);
		
		//display histogram
		//~ testHistogramImage=draw2dHistogram(testImageHistogram,30,32,10);
		testHistogramImage=draw2dHistogram(testImageHistogram,hist_Hue_bins,hist_Sat_bins,10);
		cvNamedWindow("testHistogram",CV_WINDOW_AUTOSIZE);
		cvShowImage("testHistogram",testHistogramImage);

		//goes through the src image finding Histogram equivalence
		cvThreshHist(testImageHistogram,0.15);
		cvCalcBackProjectPatch( planes, backProjection,
                             patchSize, testImageHistogram,
                             CV_COMP_BHATTACHARYYA, histogramNormalizationFactor );
                             
		IplImage * threshImage=cvCreateImage(cvGetSize(backProjection),8,1);
        cvThreshold(backProjection,threshImage,threshBackProjectionValue,255,CV_THRESH_BINARY_INV);
                       
        IplImage* eqBackProjection = cvCreateImage( cvGetSize(backProjection), 8, 1 );
        IplImage* backProjection_8b = cvCreateImage( cvGetSize(backProjection), 8, 1 );
        //~ 
        cvConvertScale( backProjection, backProjection_8b, 1, 0) ;
        //~ 

        //~ cvEqualizeHist(backProjection_8b,eqBackProjection);
        //~ cvThreshHist(testImageHistogram,100);
        
        float max_value;
        float min_value;
        int minpos[]={0,0};
        int maxpos[]={0,0};
        
        //~ printf("max pos %d,%d",maxpos[0],maxpos[1]);
        //~ printf("max pos %d,%d",minpos[0],minpos[1]);
        
        //~ cvRectangle( src, cvPoint( maxpos[1], maxpos[0] ),
						 //~ cvPoint( maxpos[1]+10, maxpos[0]+10 ),
						 //~ CV_RGB(255,255,255),CV_FILLED );
        
		
		cvNamedWindow("backProjection",CV_WINDOW_AUTOSIZE);
		cvShowImage("backProjection",backProjection);
		
		//~ cvNamedWindow("backProjectionb",CV_WINDOW_AUTOSIZE);
		//~ cvShowImage("backProjectionb",backProjection_8b);
//~ 
		cvNamedWindow("EQbackProjection",CV_WINDOW_AUTOSIZE);
		cvShowImage("EQbackProjection",eqBackProjection);
		
		cvNamedWindow("src",CV_WINDOW_AUTOSIZE);
		cvShowImage("src",src);
		
		cvNamedWindow("test",CV_WINDOW_AUTOSIZE);
		cvShowImage("test",test);
		
		cvNamedWindow("backProjectionThreshold",CV_WINDOW_AUTOSIZE);
		cvShowImage("backProjectionThreshold",threshImage);
		
		cvWaitKey();
		
	}
	
	return 0;
}

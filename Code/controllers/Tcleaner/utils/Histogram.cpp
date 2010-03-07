//      histogram.c
//      
//      Copyright 2010 dnul <dnul@alu.itba.edu.ar>
//      

#include <utils/Histogram.h>

namespace utils{

Histogram::Histogram(int h_bins,int s_bins){
	this->h_bins = h_bins;
	this->s_bins = s_bins;
}

CvHistogram * Histogram::getHShistogramFromHS(IplImage* hChannel,IplImage* sChannel){
	
	IplImage* planes[] = { hChannel, sChannel };
	
	int hist_size[] = {this->h_bins, this->s_bins};
	float h_ranges[] = { 0, 180 }; /* hue varies from 0 (~0°red) to 180 (~360°red again) */
	float s_ranges[] = { 0, 255 }; /* saturation varies from 0 (black-gray-white) to 255 (pure spectrum color) */
	float* ranges[] = { h_ranges, s_ranges };
	CvHistogram* hist;

	
	hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );
	cvCalcHist( planes, hist, 0, 0 );
	cvNormalizeHist(hist,1.0);
	
	return hist;
	
}

//default values h_bins=30,s_bins=32,scale=10
CvHistogram * Histogram::getHShistogramFromRGB(IplImage* src){
	IplImage* h_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* s_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* v_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* planes[] = { h_plane, s_plane };
	IplImage* hsv = cvCreateImage( cvGetSize(src), 8, 3 );
	
	int hist_size[] = {this->h_bins, this->s_bins};
	float h_ranges[] = { 0, 180 }; /* hue varies from 0 (~0°red) to 180 (~360°red again) */
	float s_ranges[] = { 0, 255 }; /* saturation varies from 0 (black-gray-white) to 255 (pure spectrum color) */
	float* ranges[] = { h_ranges, s_ranges };
	
	CvHistogram* hist;

	
	cvCvtColor( src, hsv, CV_BGR2HSV );
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
	hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );
	cvCalcHist( planes, hist, 0, 0 );
	cvNormalizeHist(hist,1.0);
	
	cvReleaseImage(&hsv);
	cvReleaseImage(&h_plane);
	cvReleaseImage(&s_plane);
	cvReleaseImage(&v_plane);
	
	
	return hist;
	
}

}

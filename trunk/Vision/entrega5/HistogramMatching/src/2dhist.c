#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>


CvHistogram * getHShistogram(IplImage* img,int h_bins,int s_bins,int scale);
IplImage* draw2dHistogram(CvHistogram * hist, int h_bins,int s_bins,int scale);


//~ int main( int argc, char** argv )
//~ {
    //~ IplImage* src;
    //~ if( argc == 2 && (src=cvLoadImage(argv[1], 1))!= 0)
    //~ {
        //~ int h_bins = 30, s_bins = 32;
        //~ int scale = 10;
        //~ IplImage* hist_img = cvCreateImage( cvSize(h_bins*scale,s_bins*scale), 8, 3 );
        //~ 
        //~ float max_value = 0;
        //~ int h, s;
//~ 
//~ 
		//~ CvHistogram* hist = getHShistogram(src,h_bins,s_bins,scale);
		//~ hist_img=draw2dHistogram(hist,h_bins,s_bins,scale);
		//~ 
        //~ cvNamedWindow( "Source", 1 );
        //~ cvShowImage( "Source", src );
        //~ 
        //~ cvNamedWindow( "HS", 1 );
        //~ cvShowImage( "HS", hist_img );
//~ 
//~ 
        //~ cvWaitKey(0);
    //~ }
//~ }

CvHistogram * getHShistogram(IplImage* src,int h_bins=30,int s_bins=32,int scale=10)
{
	IplImage* h_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* s_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* v_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* planes[] = { h_plane, s_plane };
	IplImage* hsv = cvCreateImage( cvGetSize(src), 8, 3 );
	
	
	
	int hist_size[] = {h_bins, s_bins};
	float h_ranges[] = { 0, 180 }; /* hue varies from 0 (~0°red) to 180 (~360°red again) */
	float s_ranges[] = { 0, 255 }; /* saturation varies from 0 (black-gray-white) to 255 (pure spectrum color) */
	float* ranges[] = { h_ranges, s_ranges };
	IplImage* hist_img = cvCreateImage( cvSize(h_bins*scale,s_bins*scale), 8, 3 );
	CvHistogram* hist;

	
	cvCvtColor( src, hsv, CV_BGR2HSV );
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
	hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );
	cvCalcHist( planes, hist, 0, 0 );
	cvNormalizeHist(hist,1.0);
	
	return hist;
	
}

CvHistogram * getHhistogram(IplImage* src,int h_bins=30,int s_bins=32,int scale=10)
{
	IplImage* h_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* s_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* v_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* planes[] = { h_plane};
	IplImage* hsv = cvCreateImage( cvGetSize(src), 8, 3 );
	
	
	
	int hist_size[] = {h_bins, s_bins};
	float h_ranges[] = { 0, 180 }; /* hue varies from 0 (~0°red) to 180 (~360°red again) */
	float* ranges[] = { h_ranges};
	CvHistogram* hist;

	
	cvCvtColor( src, hsv, CV_BGR2HSV );
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
	hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );
	cvCalcHist( planes, hist, 0, 0 );
	cvNormalizeHist(hist,1.0);
	
	return hist;
	
}

IplImage* draw2dHistogram(CvHistogram * hist, int h_bins,int s_bins,int scale)
{
	float max_value = 0;
	int max_hue=0;
	IplImage* hist_img = cvCreateImage( cvSize(h_bins*scale,s_bins*scale), 8, 3 );
	cvGetMinMaxHistValue( hist, 0, &max_value, &max_hue, 0 );
	printf("max hue:%d \n",max_hue);
	cvZero( hist_img );
	int h,s;

	for( h = 0; h < h_bins; h++ )
	{
		for( s = 0; s < s_bins; s++ )
		{
			float bin_val = cvQueryHistValue_2D( hist, h, s );
			int intensity = cvRound(bin_val*255/max_value);
			cvRectangle( hist_img, cvPoint( h*scale, s*scale ),
						 cvPoint( (h+1)*scale - 1, (s+1)*scale - 1),
						 CV_RGB(intensity,intensity,intensity),CV_FILLED );/* graw a grayscale histogram.
																   if you have idea how to do it
																   nicer let us know */
						 
		}
	}
	
	return hist_img;
    

	
	
}
//~ IplImage* drawHistogram(CvHistogram * hist, int bins,int scale)
//~ {
	//~ int hist_size = 64;
	//~ float range_0[]={0,256};
	//~ float* ranges[] = { range_0 };
	//~ float max_value;
	//~ 
	//~ IplImage* hist_img = cvCreateImage( cvSize(h_bins*scale,s_bins*scale), 8, 3 );
	//~ 
	//~ cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );
    //~ cvScale( hist->bins, hist->bins, ((double)hist_image->height)/max_value, 0 );
    //~ 
    //~ bin_w = cvRound((double)hist_image->width/hist_size);
//~ 
	 //~ for( i = 0; i < hist_size; i++ )
        //~ cvRectangle( hist_image, cvPoint(i*bin_w, hist_image->height),
                     //~ cvPoint((i+1)*bin_w, hist_image->height - cvRound(cvGetReal1D(hist->bins,i))),
                     //~ cvScalarAll(0), -1, 8, 0 );
                     //~ 
                     //~ 
	//~ 
	//~ return hist_img;
    //~ 
//~ 
	//~ 
	//~ 
//~ }
//~ 

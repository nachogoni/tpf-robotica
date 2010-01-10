from opencv.cv import *
from opencv.highgui import *
import sys


frame=None
hist_size = 256
range_0=[0,256]
ranges = [ range_0 ]
hist_image=None
hist=None

#hsv image and channels
hsv_image=None
hhsv_image=None
shsv_image=None
vhsv_image=None


def genHist(hist_image,hist_size,hist):
    for i in range(hist_size):
        cvRectangle( hist_image, cvPoint(i*bin_w, hist_image.height),
                     cvPoint((i+1)*bin_w, hist_image.height - cvRound(cvGetReal1D(hist.bins,i))),
                     cvScalarAll(0), -1, 8, 0 );


if __name__ == '__main__':
    cvNamedWindow("histogram",CV_WINDOW_AUTOSIZE)
    cvNamedWindow("image",CV_WINDOW_AUTOSIZE)
    
    capture=cvCreateFileCapture("/media/My_Passport/Videos/colillas-final2.avi")
    frame=cvQueryFrame(capture)
    
    
    #hsv initialization
    hsv_image=cvCloneImage(frame)
    hhsv_image=cvCreateImage(cvGetSize(frame),frame.depth,1)
    
    #histogram initialization
    hist_image = cvCreateImage(cvSize(320,200), 8, 1);
    hist = cvCreateHist([hist_size], CV_HIST_ARRAY, ranges, 1);
    
    while(True):
        frame=cvQueryFrame(capture)
        #convert to hsv
        cvCvtColor(frame,hsv_image,CV_RGB2HSV)
        #split channels
        cvSplit(hsv_image,hhsv_image,shsv_image,vhsv_image,None)
        
        cvCalcHist( hhsv_image, hist, 0, None );
        
        #get histogram bin width
        min_value, max_value = cvGetMinMaxHistValue( hist );
        cvScale( hist.bins, hist.bins, float(hist_image.height)/max_value, 0 );
        #cvNormalizeHist( hist, 1000 );
        
        cvSet( hist_image, cvScalarAll(255));
        bin_w = cvRound(float(hist_image.width)/hist_size);
        
        genHist(hist_image, hist_size, hist)
        
        cvShowImage("image",frame)
        cvShowImage("histogram",hist_image)
        cvWaitKey()
        

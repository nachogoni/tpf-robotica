from opencv.cv import *
from opencv import cv
from opencv.highgui import *
from contourFilters import areaFilter, rectangularAspectFilter, boxAreaFilter,\
    perimeterFilter


MORPH_KERNEL_SIZE=2
DILATE_ITER=2
ERODE_ITER=2

#Polygon Contour aproximation tolerance
PER_TOLERANCE=50

#contour constants
CURRENT_CONTOUR = 3
CONTOUR_THICKNESS=3  

#color constants
_red = cv.cvScalar (0, 0, 255, 0);
_green = cv.cvScalar (0, 255, 0, 0);

def getPatchContour():
    frame=cvLoadImage("../images/colilla-patch2.png")
    src_hsvImage=cvCreateImage(cvGetSize(frame),frame.depth,3)
    src_Himage=cvCreateImage(cvGetSize(frame),frame.depth,1)
    src_Simage=cvCreateImage(cvGetSize(frame),frame.depth,1)
    src_Vimage=cvCreateImage(cvGetSize(frame),frame.depth,1)
    cvCvtColor(frame,src_hsvImage,CV_BGR2HSV)
    cvSplit(src_hsvImage,src_Himage,src_Simage,src_Vimage,None)
    cvShowImage("HSV",src_Simage)    
    
    eqImage=cvClone(src_Simage)
    cvEqualizeHist(src_Simage,eqImage)
    cvShowImage("equalized",eqImage)
    
    
    #thresholding
    threshImage=cvCreateImage(cvGetSize(frame),8,1)
    cvThreshold(src_Simage,threshImage,100,255,CV_THRESH_BINARY)
    cvShowImage("threshold",threshImage)
    
    
    morphInput=threshImage
    morphOutput=cvCloneImage(threshImage)
    
    element = cvCreateStructuringElementEx( MORPH_KERNEL_SIZE*2+1, MORPH_KERNEL_SIZE*2+1, MORPH_KERNEL_SIZE, MORPH_KERNEL_SIZE, CV_SHAPE_RECT, None );
    cvDilate(morphInput,morphOutput,element,DILATE_ITER);
    cvErode(morphOutput,morphOutput,element,ERODE_ITER);
    cvShowImage("Morph window",morphOutput);
    
    
    contoursInput=morphOutput   
    contoursOutput=cvCloneImage(frame)
    
    contours=findContours(contoursInput,True)
    #apply filters
    if contours!=None:
        #boxarea filter
        

        contours=filter(areaFilter,contours)
        contours=filter(perimeterFilter,contours)
        contours=filter(rectangularAspectFilter,contours)
        contours=filter(boxAreaFilter,contours)
        
        #finally, draw remaining contours
        for contour in contours:
            return contour
    
def findContours(image,getPolygon):
    storage = cv.cvCreateMemStorage (0)
    polyContourArray=[]
    polyStorage=cv.cvCreateMemStorage (0)
    nb_contours, contours = cv.cvFindContours (image,
                                               storage,
                                               cv.sizeof_CvContour,
                                               cv.CV_RETR_TREE,
                                               cv.CV_CHAIN_APPROX_SIMPLE,
                                               cv.cvPoint (0,0))
    
    if contours==None:
        return None
    
    contoursList=list(contours.hrange())
    if not getPolygon:
        ret=contoursList
    else:
        for contour in contoursList:
            per=cvContourPerimeter(contour)
            polyContourArray.append(cv.cvApproxPoly (contour, cv.sizeof_CvContour,
                                    storage,
                                    cv.CV_POLY_APPROX_DP, per/PER_TOLERANCE, 0))
        ret=polyContourArray
    
    return ret
def printContour(contour,position,color,imagen):
    #print " a contour " +  str(contour.total) + " area: " + str(cvContourArea(contour)) + " perimeter: " + str(cvContourPerimeter(contour))

    # compute the real level of display, given the current position
    levels = position - 3

        
    
    # draw contours in red and green
    cv.cvDrawContours (imagen, contour,
                       color, _green,
                       levels, CONTOUR_THICKNESS, cv.CV_AA,
                       cv.cvPoint (0, 0))

       




if __name__ == '__main__':
    cvNamedWindow("frame",CV_WINDOW_AUTOSIZE)
    cvNamedWindow("HSV",CV_WINDOW_AUTOSIZE)
    cvNamedWindow("threshold",CV_WINDOW_AUTOSIZE)
    cvNamedWindow("equalized",CV_WINDOW_AUTOSIZE)
    cvNamedWindow("Morph window",CV_WINDOW_AUTOSIZE)
    cvNamedWindow("Contours",CV_WINDOW_AUTOSIZE)
    
    
    patch=getPatchContour()
    #capture=cvCreateFileCapture("/home/diego/Videos/colillas-final.avi")
    capture=cvCreateFileCapture("/media/My_Passport/Videos/colillas-final2.avi")
    img=cvLoadImage("../images/colilla-gris1.png",CV_LOAD_IMAGE_GRAYSCALE)
    
    for i in range(200):
        cvQueryFrame(capture)
    
    while(True):
        frame=cvQueryFrame(capture)
        cvShowImage("frame",frame)
        src_hsvImage=cvCreateImage(cvGetSize(frame),frame.depth,3)
        src_Himage=cvCreateImage(cvGetSize(frame),frame.depth,1)
        src_Simage=cvCreateImage(cvGetSize(frame),frame.depth,1)
        src_Vimage=cvCreateImage(cvGetSize(frame),frame.depth,1)
        cvCvtColor(frame,src_hsvImage,CV_BGR2HSV)
        cvSplit(src_hsvImage,src_Himage,src_Simage,src_Vimage,None)
        cvShowImage("HSV",src_Simage)    
        
        eqImage=cvClone(src_Simage)
        cvEqualizeHist(src_Simage,eqImage)
        cvShowImage("equalized",eqImage)
        
        
        #thresholding
        threshImage=cvCreateImage(cvGetSize(frame),8,1)
        cvThreshold(eqImage,threshImage,240,255,CV_THRESH_BINARY)
        cvShowImage("threshold",threshImage)
        
        
        morphInput=threshImage
        morphOutput=cvCloneImage(threshImage)
        
        element = cvCreateStructuringElementEx( MORPH_KERNEL_SIZE*2+1, MORPH_KERNEL_SIZE*2+1, MORPH_KERNEL_SIZE, MORPH_KERNEL_SIZE, CV_SHAPE_RECT, None );
        cvDilate(morphInput,morphOutput,element,DILATE_ITER);
        cvErode(morphOutput,morphOutput,element,ERODE_ITER);
        cvShowImage("Morph window",morphOutput);
        
        
        contoursInput=morphOutput   
        contoursOutput=cvCloneImage(frame)
        
        contours=findContours(contoursInput,True)
        #apply filters
        if contours!=None:
            #boxarea filter
            

            contours=filter(areaFilter,contours)
            contours=filter(perimeterFilter,contours)
            contours=filter(rectangularAspectFilter,contours)
            contours=filter(boxAreaFilter,contours)
            
            #finally, draw remaining contours
            for contour in contours:
                printContour(contour, CURRENT_CONTOUR, cvScalar(127,127,0),contoursOutput)
                printContour(patch, CURRENT_CONTOUR, cvScalar(100,127,0),contoursOutput)
                print "method 1:" + str(cvMatchShapes(contour,patch,1))
                print "Method 2:"  + str(cvMatchShapes(contour,patch,2))
                print "method 3:" + str(cvMatchShapes(contour,patch,3))
#                print "method 1:" + str(cvMatchShapes(patch,patch,1))
#                print "Method 2:"  + str(cvMatchShapes(patch,patch,2))
#                print "method 3:" + str(cvMatchShapes(patch,patch,3))
         
        
            #display Contour results  
            cvShowImage("Contours",contoursOutput)

        
        
        cvWaitKey()

from opencv.cv import *
from opencv import cv
from opencv.highgui import *
from contourFilters import areaFilter, rectangularAspectFilter, boxAreaFilter,\
    perimeterFilter
from filterBySaturation import _red

#roi
currentRoi=None
currentRoiRect=None

DO_WINDOWING=False

MORPH_KERNEL_SIZE=2
#DILATE_ITER=2
#ERODE_ITER=2
DILATE_ITER=1
ERODE_ITER=1

#Polygon Contour aproximation tolerance
PER_TOLERANCE=50

#contour constants
CURRENT_CONTOUR = 2
CONTOUR_THICKNESS=2  

#threshold values
THRESHOLD_VALUE=240

#color constants
_red = cv.cvScalar (0, 0, 255, 0);
_green = cv.cvScalar (0, 255, 0, 0);

def isValidRect(rect,fullFrame):
    if rect.x<0 or rect.y <0:
        return False
    if rect.x>fullFrame.width or rect.y>fullFrame.height:
        return False
    if rect.x+rect.width>fullFrame.width or rect.y+rect.height>fullFrame.height:
        return False
    
    return True
    
def biggerRect(rect,percent,fullFrame):
    newRect=cvRect(rect.x-rect.width/2,rect.y-rect.height/2,
                  rect.width*2,rect.height*2)
    
    if newRect.x<0:
        newRect.x=0
    if newRect.y<0:
        newRect.y=0
    if newRect.x+newRect.width>fullFrame.width:
        newRect.width=fullFrame.width - newRect.x -1
    if newRect.y+newRect.height>fullFrame.height:
        newRect.height=fullFrame.height - newRect.y -1
    return newRect
    
def drawBiggerBbox(rect,outputImage,fullFrame):
    bigRect=biggerRect(rect,100)
    cvRectangle(outputImage,cvPoint(bigRect.x,bigRect.y),
                cvPoint(bigRect.x+bigRect.width,bigRect.y+bigRect.height),
                _red)

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
    #cvNamedWindow("frame",CV_WINDOW_AUTOSIZE)
    cvNamedWindow("HSV",CV_WINDOW_AUTOSIZE)
    cvNamedWindow("threshold",CV_WINDOW_AUTOSIZE)
    cvNamedWindow("equalized",CV_WINDOW_AUTOSIZE)
    cvNamedWindow("Morph window",CV_WINDOW_AUTOSIZE)
    cvNamedWindow("Contours",CV_WINDOW_AUTOSIZE)
    cvNamedWindow("Smooth",CV_WINDOW_AUTOSIZE)
    #cvNamedWindow("FullFrame",CV_WINDOW_AUTOSIZE)
    
    #checks if an object is present in the current frame
    foundObject=False
    
    currentRoiRect=None
    fullFrame=None
    
    #capture=cvCreateFileCapture("/home/diego/Videos/colillas-final2.avi")
    #capture=cvCreateFileCapture("/home/diego/Videos/output.avi")
    #capture=cvCreateFileCapture("/home/diego/Videos/primerpaneo2.wmv")
    capture=cvCreateFileCapture("/media/My_Passport/Videos/colillas-360.wmv")
    img=cvLoadImage("../images/colilla-gris1.png",CV_LOAD_IMAGE_GRAYSCALE)
    #skip frames
    for i in range(0):
        frame=cvQueryFrame(capture)
    
    
    frameCount=0
    
    while(True):
        frame=cvQueryFrame(capture)
        fullFrame=cvCloneImage(frame)
        frameCount+=1
        print frameCount
        
        #roi management
        if currentRoiRect!=None:
            print str(currentRoiRect.x) + " " + str(currentRoiRect.y) + " " + str(currentRoiRect.width) + " " + str(currentRoiRect.height ) 
            currentRoi=cvCreateImage(cvSize(currentRoiRect.width,
                                            currentRoiRect.height),
                                            frame.depth,frame.nChannels)
            
            
            currentRoi=cvGetSubRect(frame,currentRoiRect)
            frame=currentRoi
        
        
        #Color extraction
        #cvShowImage("frame",frame)
        src_hsvImage=cvCreateImage(cvGetSize(frame),frame.depth,3)
        src_Himage=cvCreateImage(cvGetSize(frame),frame.depth,1)
        src_Simage=cvCreateImage(cvGetSize(frame),frame.depth,1)
        src_Vimage=cvCreateImage(cvGetSize(frame),frame.depth,1)
        cvCvtColor(frame,src_hsvImage,CV_BGR2HSV)
        cvSplit(src_hsvImage,src_Himage,src_Simage,src_Vimage,None)
        cvShowImage("HSV",src_Simage)    
        
        #Image Equalization
        eqImage=cvClone(src_Simage)
        cvEqualizeHist(src_Simage,eqImage)
        cvShowImage("equalized",eqImage)
        
        
        #thresholding
        threshImage=cvCreateImage(cvGetSize(frame),8,1)
        
#        cvAdaptiveThreshold(img,threshImage,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY_INV,31)
#        cvThreshold(src_Simage,threshImage,THRESHOLD_VALUE,255,CV_THRESH_BINARY)
        cvThreshold(eqImage,threshImage,THRESHOLD_VALUE,255,CV_THRESH_BINARY)
        cvShowImage("threshold",threshImage)
        
        
        
        #Morphology
        morphInput=threshImage
        morphOutput=cvCloneImage(threshImage)
        
        element = cvCreateStructuringElementEx( MORPH_KERNEL_SIZE*2+1, MORPH_KERNEL_SIZE*2+1, MORPH_KERNEL_SIZE, MORPH_KERNEL_SIZE, CV_SHAPE_RECT, None );
        cvDilate(morphInput,morphOutput,element,DILATE_ITER);
        cvErode(morphOutput,morphOutput,element,ERODE_ITER);
        cvShowImage("Morph window",morphOutput);
        
        
        #Smoothen
        smoothOutput=cvCloneImage(morphOutput)
        cvSmooth(morphOutput,smoothOutput)
        cvShowImage("Smooth",smoothOutput)
        
        
        #Countour finding
        contoursInput=smoothOutput   
        contoursOutput=cvCloneImage(frame)
        
     
        
        contours=findContours(contoursInput,True)
        #contours=findContours(smoothOutput,True)
        #apply filters
        if contours!=None:
              
#            contours=filter(areaFilter,contours)
            contours=filter(perimeterFilter,contours)
            contours=filter(rectangularAspectFilter,contours)
            contours=filter(boxAreaFilter,contours)
            
            if contours==[]:
                foundObject=False
            else:
                foundObject=True
            
            #finally, draw remaining contours
            for contour in contours:
                printContour(contour, CURRENT_CONTOUR, cvScalar(127,127,0),contoursOutput)
                
                #draw bounding box
                boundingRect=cvBoundingRect(contour,0)
                cvRectangle(contoursOutput,cvPoint(boundingRect.x,boundingRect.y),
                            cvPoint(boundingRect.x+boundingRect.width,boundingRect.y+boundingRect.height),
                            _green)
                
                
                if DO_WINDOWING:
                    #draw bigger bounding box for windowing
                    if currentRoiRect==None:
                        currentRoiRect=biggerRect(boundingRect,100,fullFrame)
                        cvRectangle(contoursOutput,cvPoint(currentRoiRect.x,currentRoiRect.y),
                        cvPoint(currentRoiRect.x+currentRoiRect.width ,currentRoiRect.y+currentRoiRect.height),
                        _red)
                    else:
                        newRoiRect=biggerRect(boundingRect,100,fullFrame)
                        #translate coordinates
                        newRoiRect.x=newRoiRect.x + currentRoiRect.x
                        newRoiRect.y=newRoiRect.y + currentRoiRect.y
                        if isValidRect(newRoiRect, fullFrame):
                            currentRoiRect=newRoiRect  
                    
               
               
               
            #if no contours are found stop windowing
            if not foundObject:
                currentRoiRect=None
            
         
         
        
            #display Contour results  
            cvShowImage("Contours",contoursOutput)
            
            
        
        #adaptive Thresholding
#        threshImage=cvCreateImage(cvGetSize(frame),8,1)
#        cvAdaptiveThreshold(eqImage,threshImage,255,CV_ADAPTIVE_THRESH_GAUSSIAN_C,CV_THRESH_BINARY,21)
#        cvShowImage("threshold",threshImage)
        
        
        cvWaitKey()

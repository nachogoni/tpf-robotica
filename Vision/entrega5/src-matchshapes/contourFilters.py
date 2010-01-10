from opencv.cv import cvCreateMemStorage,cvMinAreaRect2,CvBox2D, cvContourArea, cvSize2D32f, cvContourPerimeter
    
MINCONTOUR_AREA=1000
BOXFILTER_TOLERANCE=0.6
#MINCONTOUR_PERIMETER=40MINCONTOUR_PERIMETER=80
CONTOUR_RECTANGULAR_MIN_RATIO=1.2CONTOUR_RECTANGULAR_MAX_RATIO=3.0

mem=cvCreateMemStorage(0)


def rectangularAspectFilter(contour):
    ret=False
    
    try:
        box=cvMinAreaRect2(contour,mem)
        if box.size.width > CONTOUR_RECTANGULAR_MIN_RATIO* box.size.height and\         box.size.width < CONTOUR_RECTANGULAR_MAX_RATIO* box.size.height\         or box.size.height > CONTOUR_RECTANGULAR_MIN_RATIO* box.size.width and\          box.size.height < CONTOUR_RECTANGULAR_MAX_RATIO* box.size.width:         
            ret=True
        else:             #print("No paso el rectangular Fitlter")
             ret=False
    except:
        pass
    return ret
    

def perimeterFilter(contour):
    ret=False
    
    try:
        ret=abs(cvContourPerimeter(contour))> MINCONTOUR_PERIMETER
    except:
        pass
    return ret

def areaFilter(contour):
    ret=False
    
    try:
        ret=abs(cvContourArea(contour))> MINCONTOUR_AREA
    except:
        pass
    return ret
       

def boxAreaFilter(contour):
    #checks if a contour area is similar to the bounding box containing that contour
    ret=False
    
    try:
        box=cvMinAreaRect2(contour,mem)
        boxArea=box.size.width*box.size.height
        contourArea=abs(cvContourArea(contour))
        dif=boxArea-contourArea
        ret=(1- (dif/boxArea))
        #calcs the percentage of the boxarea that the contourarea covers        #print("contourArea: " + str(contourArea) + "boxarea: " + str(boxArea)) 
    except:
        pass
     
    if ( ret > BOXFILTER_TOLERANCE):       return True    else:       #print("No paso el boxarea filter")       return False      
      
    
    
    
    



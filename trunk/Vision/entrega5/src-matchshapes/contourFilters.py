
    
MINCONTOUR_AREA=1000
BOXFILTER_TOLERANCE=0.6
#MINCONTOUR_PERIMETER=40
CONTOUR_RECTANGULAR_MIN_RATIO=1.2

mem=cvCreateMemStorage(0)


def rectangularAspectFilter(contour):
    ret=False
    
    try:
        box=cvMinAreaRect2(contour,mem)
        if box.size.width > CONTOUR_RECTANGULAR_MIN_RATIO* box.size.height and\
            ret=True
        else:
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
        #calcs the percentage of the boxarea that the contourarea covers
    except:
        pass
     
    if ( ret > BOXFILTER_TOLERANCE):
      
    
    
    
    


#!/usr/bin/env python

import cv
import time
from objectBox import ObjectBox
import collections
import xmlObjects
import sys
import getopt
from copy import deepcopy

def is_rect_nonzero(r):
    (_,_,w,h) = r
    return (w > 0) and (h > 0)
def moveObjects(objectList,(x,y)):
	for object in objectList:
		object.move(x,y)
	return objectList
		
	

class CamShiftDemo:

    def __init__(self,filename):
        #self.capture = cv.CaptureFromCAM(0)
        #self.capture=cv.CaptureFromFile('/home/diego/Videos/colillas-2010.mpg');
        self.capture=cv.CaptureFromFile(filename)
        print "fps" + str(cv.GetCaptureProperty(self.capture,cv.CV_CAP_PROP_FPS))
        if self.capture==0:
            raise Exception
        cv.NamedWindow( "Output", 1 )
        cv.SetMouseCallback( "Output", self.on_mouse)
        
        self.filename=filename
        self.startFrame=1
        self.drag_start = None      # Set to (x,y) when mouse starts drag
        self.track_window = None    # Set to rect when the mouse drag finishes
        self.selection=None
        self.drag_end=False
        self.objects={} 			# Dictionary containing the position of objects
        self.frameNumber=0
        print( "Keys:\n"
            "    ESC - quit the program\n"
            "    b - switch to/from backprojection view\n"
            "To initialize tracking, drag across the object with the mouse\n" )

    def on_mouse(self, event, x, y, flags, param):
        if event == cv.CV_EVENT_LBUTTONDOWN:
			self.drag_start = (x, y)
        	   
        if event == cv.CV_EVENT_LBUTTONUP:
            if self.drag_start!=None: 
            	if is_rect_nonzero(self.selection):
                    self.objects[self.frameNumber].append(ObjectBox(self.selection))
                    self.track_window = self.selection
                else:
                	#remove object
                	remove=self.objects[self.frameNumber][:]
                	for object in remove:
                		if object.containsPoint((x,y)):
                			self.objects[self.frameNumber].remove(object)
            	
            self.drag_start = None
            self.drag_end=True
            
        if self.drag_start:
            xmin = min(x, self.drag_start[0])
            ymin = min(y, self.drag_start[1])
            xmax = max(x, self.drag_start[0])
            ymax = max(y, self.drag_start[1])
            self.selection = (xmin, ymin, xmax - xmin, ymax - ymin)

    def run(self):

    	self.objects[0]=list()
    	
        while True:
            frame = cv.QueryFrame( self.capture)
            if frame==0:
                break
            self.frameNumber=self.frameNumber+1
            
            if self.frameNumber%10 == 0:
                self.save()
                
            #copy last frame objects
            self.objects[self.frameNumber]=deepcopy(self.objects[self.frameNumber-1][:])
            
                
            
          
            while True:
            	frameSelection = cv.CloneImage(frame)
            	for object in self.objects[self.frameNumber]:
            		x,y,w,h = object.getRectangle()
            		cv.Rectangle(frameSelection, (x,y), (x+w,y+h), (255,255,255))
        	   
        	   # If mouse is pressed, highlight the current selected rectangle
        	   # and recompute the histogram            
                if self.drag_start and is_rect_nonzero(self.selection):
               
                    sub = cv.GetSubRect(frameSelection, self.selection)
                    save = cv.CloneMat(sub)
                    cv.ConvertScale(frameSelection, frameSelection, 0.5)
                    cv.Copy(save, sub)
                    x,y,w,h = self.selection
                    cv.Rectangle(frameSelection, (x,y), (x+w,y+h), (255,255,255))

            	cv.ShowImage("Output", frameSelection)
            	key=cv.WaitKey(1000/25)
            	if key == ord("n"):
                    break
                if key==ord("w"):
                    self.save()
                if key==ord("s"):
                    self.startFrame=self.frameNumber
                    print "Set Starting frame: " + self.frameNumber
                if key==ord("c"):
                	self.objects[self.frameNumber].extend(self.objects[self.frameNumber-1][:])
                if key==ord("d"):
                	self.objects[self.frameNumber]=list()
                elif key==65364:
                	moveObjects(self.objects[self.frameNumber],(0,1))
                elif key==65362:
                	moveObjects(self.objects[self.frameNumber],(0,-1))
                elif key==65363:
                	moveObjects(self.objects[self.frameNumber],(1,0))
                elif key==65361:
                	moveObjects(self.objects[self.frameNumber],(-1,0))
                	
                	#65364

    def save(self):
        dict=self.objects.copy()
        for l in self.objects:
            if dict[l]==[]:
                dict.pop(l)
        name=self.filename.rsplit("/")[-1]
        xmlObjects.saveObjects(dict, 'a', name, self.capture,self.frameNumber)

def main(argv):
    try:                                
        opts, args = getopt.getopt(argv, "v:h", ["video=", "help"]) 
    except getopt.GetoptError:           
        usage()                          
        sys.exit(2)
    
    for opt,arg in opts:
        if opt in(["-v","--video"]):
            filename=arg
    
    print filename
    
#    try:
    demo = CamShiftDemo(filename)
    demo.run()
#    except Exception as e:
#        print type(e)
    print "invalid capture"
#        print type(e)
    sys.exit(2)
        
    #demo.run()

    
         

def usage():
    print "Usage:\n"
    print "-v .. \n" + "    Video to edit\n" 




if __name__=="__main__":
    main(sys.argv[1:])

    #demo = CamShiftDemo()
    #demo.run()


    

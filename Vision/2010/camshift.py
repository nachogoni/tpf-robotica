#!/usr/bin/env python

import cv

def is_rect_nonzero(r):
    (_,_,w,h) = r
    return (w > 0) and (h > 0)

class CamShiftDemo:

    def __init__(self):
        #self.capture = cv.CaptureFromCAM(0)
        #self.capture=cv.CaptureFromFile('/home/diego/Videos/colillas-2010.mpg');
        self.capture=cv.CaptureFromFile('/home/diego/Videos/primerpaneo2.wmv');
        cv.NamedWindow( "Output", 1 )
        cv.SetMouseCallback( "Output", self.on_mouse)

        self.drag_start = None      # Set to (x,y) when mouse starts drag
        self.track_window = None    # Set to rect when the mouse drag finishes
        self.objects={} 			# Dictionary containing the position of objects

        print( "Keys:\n"
            "    ESC - quit the program\n"
            "    b - switch to/from backprojection view\n"
            "To initialize tracking, drag across the object with the mouse\n" )

    def on_mouse(self, event, x, y, flags, param):
        if event == cv.CV_EVENT_LBUTTONDOWN:
            self.drag_start = (x, y)
        if event == cv.CV_EVENT_LBUTTONUP:
            self.drag_start = None
            self.track_window = self.selection
        if self.drag_start:
            xmin = min(x, self.drag_start[0])
            ymin = min(y, self.drag_start[1])
            xmax = max(x, self.drag_start[0])
            ymax = max(y, self.drag_start[1])
            self.selection = (xmin, ymin, xmax - xmin, ymax - ymin)

    def run(self):
		
        while True:
            frame = cv.QueryFrame( self.capture)
            
			# Run the cam-shift
            if self.track_window and is_rect_nonzero(self.track_window):
                crit = ( cv.CV_TERMCRIT_EPS | cv.CV_TERMCRIT_ITER, 10, 1)
                (iters, (area, value, rect), track_box) = cv.CamShift(backproject, self.track_window, crit)
                self.track_window = rect

            # If mouse is pressed, highlight the current selected rectangle
            # and recompute the histogram

            if self.drag_start and is_rect_nonzero(self.selection):
				sub = cv.GetSubRect(frame, self.selection)
				save = cv.CloneMat(sub)
				cv.ConvertScale(frame, frame, 0.5)
				cv.Copy(save, sub)
				x,y,w,h = self.selection
				cv.Rectangle(frame, (x,y), (x+w,y+h), (255,255,255))

				sel = cv.GetSubRect(self.hue, self.selection )
				cv.CalcArrHist( [sel], hist, 0)
				(_, max_val, _, _) = cv.GetMinMaxHistValue( hist)
				if max_val != 0:
					cv.ConvertScale(hist.bins, hist.bins, 255. / max_val)        
				elif self.track_window and is_rect_nonzero(self.track_window):
					cv.EllipseBox( frame, track_box, cv.CV_RGB(255,0,0), 3, cv.CV_AA, 0 )		
	   	    cv.ShowImage("Output",frame)
	   	   	cv.WaitKey(0)
			
if __name__=="__main__":
    demo = CamShiftDemo()
    demo.run()

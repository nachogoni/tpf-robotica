#!/usr/bin/env python


class ObjectBox:
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

	
	return 0

if __name__ == '__main__':
	main()

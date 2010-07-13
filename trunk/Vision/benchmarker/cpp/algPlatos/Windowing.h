#ifndef utils_Windowing_h
#define utils_Windowing_h

#include "GarbageHistoric.h"
#include <vector>
#include <list>
#include <highgui.h>
#include <cv.h>
#include "Garbage.h"
#include "PredictionParameters.h"



namespace utils{

	class Windowing {
		
	 public:
		~Windowing();
		Windowing();
		Windowing(GarbageHistoric * focusGarbage,CvSize srcSize);
		bool isValidWindow(CvRect newWindow);
		void enlargeWindow();
		IplImage * getWindow(IplImage * src);
		CvRect adjustWindow(CvRect rect);
		std::list<Garbage*> correctGarbages(std::list<Garbage*> garbageList);
		
		//variables
		CvRect window;
		CvSize srcSize;
		int frameNumber;
		GarbageHistoric * currentGarbage;
		IplImage * windowImage,*last_window,*last_last_window;
		CvMat * tmpMat;
		bool release_window;
	     
	     	
	};

}

#endif // utils_Windowing_h

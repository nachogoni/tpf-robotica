#ifndef GarbageAdapter_h
#define GarbageAdapter_h

#include <stdio.h>
#include "Cobject.h"
#include "GarbageRecognition.h"
#include <cv.h>
#include <list>


	

class GarbageAdapter{
	
	public:
		std::list<benchmark::Cobject*> recognizeObjects(IplImage * img);
		utils::GarbageRecognition * gr;
		GarbageAdapter();
		~GarbageAdapter();
	
	
	
};

#endif

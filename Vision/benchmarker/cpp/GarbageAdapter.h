#ifndef GarbageAdapter_h
#define GarbageAdapter_h

#include <stdio.h>
#include "Cobject.h"
#include <cv.h>
#include <list>


	

class GarbageAdapter{
	
	public:
	static std::list<benchmark::Cobject*> recognizeObjects(IplImage * img);
	GarbageAdapter();
	~GarbageAdapter();
	
	
	
};

#endif

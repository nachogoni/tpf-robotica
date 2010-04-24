#include <stdio.h>
#include <iostream>
#include <list>
#include "Cobject.h"

#ifndef FRAMEADT_H_
#define FRAMEADT_H_

#define MAX_FRAME_OBJECTS 10

namespace benchmark{

class Frame
{
	public:
		Frame(int frameNumber);
		~Frame();
		void addObject(Cobject *obj);
		int getNumberOfObjects();
		std::list<Cobject*> objectList;
		int frameNumber;

};

}

#endif /*FRAMEADT_H_*/


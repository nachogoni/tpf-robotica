#include <stdio.h>
#include <stdlib.h>
#include "frameADT.h"
#include "listADT.h"
#include "cObjectADT.h"



namespace benchmark{
	


Frame(int frameNumber){
	this->frameNumber=frameNumber;
	
}

~Frame(){
	
}

void 
Frame::addObject(Cobject obj){
	if(obj!=NULL)
		this->objectList.push_back(obj);
	
}

int
Frame::getNumberOfObjects(frameADT f)
{
	return this->objectList.size();
}

}







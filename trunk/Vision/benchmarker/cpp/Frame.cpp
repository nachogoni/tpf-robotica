#include <stdio.h>
#include <stdlib.h>
#include <list>
#include "Frame.h"
#include "Cobject.h"



namespace benchmark{
	


Frame::Frame(int frameNumber){
	this->frameNumber=frameNumber;
	
}

Frame::~Frame(){
	
}

void 
Frame::addObject(Cobject* obj){
	this->objectList.push_back(obj);
}

int
Frame::getNumberOfObjects()
{
	return this->objectList.size();
}
std::list<Cobject*> Frame::getObjects(){
	return this->objectList;
}


}







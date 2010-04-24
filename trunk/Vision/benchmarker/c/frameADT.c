#include <stdio.h>
#include <stdlib.h>
#include "frameADT.h"
#include "listADT.h"
#include "cObjectADT.h"



struct frameCDT
{
	int frameNumber;
	cObjectADT objects[MAX_FRAME_OBJECTS];
	int numberOfObjects;
};



frameADT 
newFrame(int frameNumber){
	frameADT auxi;

	if ( (auxi = malloc(sizeof(struct frameCDT))) == NULL)
		return NULL;
		
	auxi->frameNumber=frameNumber;
	auxi->numberOfObjects=0;
	
	return auxi;
}

void
addObject(frameADT frame,cObjectADT obj){
	
	if(frame==NULL || frame->numberOfObjects>=MAX_FRAME_OBJECTS)
		return;
	
	frame->objects[frame->numberOfObjects++]=obj;
	
	return;
}

int
getNumberOfObjects(frameADT f)
{
	if(f!=NULL){
		return f->numberOfObjects;
	}
	
	return -1;
	
}





#include <stdio.h>
#ifndef FRAMEADT_H_
#define FRAMEADT_H_

#define MAX_FRAME_OBJECTS 10

struct frameCDT
{
	int frameNumber;
	cObjectADT objects[MAX_FRAME_OBJECTS];
	int numberOfObjects;
};


typedef struct frameCDT* frameADT;

frameADT newFrame();

int
getNumberOfObjects(frameADT f);

#endif /*FRAMEADT_H_*/


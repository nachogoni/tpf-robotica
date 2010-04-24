#include <stdlib.h>
#include <stdio.h>
#include "listADT.h"
#include "xmlParser.h"
#include "frameADT.h"
#include "cObjectADT.h"

int main(int argc, char** argv)
{
	frameADT frame;
	int frameCounter=1,i;
	int nextFrame;
	
	char  s[10];
	if (argc < 2) {
        fprintf(stderr, "Usage: %s filename.xml\n", argv[0]);
        return 1;
    }
    
    listADT framesList=parseXmlObjects(argv[1]);
    
    SetBegin(framesList);
    GetDato(framesList,&frame,s)==1
    nextFrame=frame->frameNumber;
    
    
    
    
    
    
    
    
    
    
    
	return 0;
}

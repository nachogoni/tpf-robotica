
#include "cObjectADT.h"
#include <stdio.h>
#include <stdlib.h>



struct cObjectCDT{
	int frame;
	int w;
	int h;
	int x;
	int y;
};

cObjectADT newCobject(int x,int y,int w,int h){
	cObjectADT instance;
	if((instance=malloc(sizeof(struct cObjectCDT)))==NULL)
		return NULL;
		
	instance->x=x;
	instance->y=y;
	instance->w=w;
	instance->h=h;
	instance->frame=0;
	
	return instance;
}

void print(cObjectADT obj){

 printf("Object in x: %d, y: %d  w: %d, h: %d\n",obj->x,obj->y,obj->w,obj->h);

}




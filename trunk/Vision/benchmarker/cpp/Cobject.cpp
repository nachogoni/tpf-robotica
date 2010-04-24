
#include "Cobject.h"
#include <stdio.h>
#include <stdlib.h>



namespace benchmark{


Cobject::Cobject(int x,int y,int w,int h)
{
	this->x=x;
	this->y=y;
	this->w=w;
	this->h=h;
	
}

Cobject::~Cobject()
{
	this->x=x;
	this->y=y;
	this->w=w;
	this->h=h;
}

void 
Cobject::print(){

 printf("Object in x: %d, y: %d  w: %d, h: %d\n",this->x,this->y,this->w,this->h);
 
}



}

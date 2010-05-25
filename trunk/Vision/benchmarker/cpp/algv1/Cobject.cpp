
#include "Cobject.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#define EPSILON 75 


namespace benchmark{

Cobject::Cobject(int index,int x,int y,int w,int h)
{
	this->index=index;
	this->x=x;
	this->y=y;
	this->w=w;
	this->h=h;
	
}

Cobject::Cobject(int index,int x,int y,int w,int h,std::vector<int> centroid)
{
	this->index=index;
	this->x=x;
	this->y=y;
	this->w=w;
	this->h=h;
	this->centroid=centroid;
	
}


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

std::vector<int> Cobject::getCentroid(){
	return this->centroid;
}
//other is xml object
bool Cobject::isSimilar(Cobject * other){
	std::vector<int> centroid1=this->getCentroid();
	std::vector<int> centroid2=other->getCentroid();
	double factor=1.0;
	
	if(other->x<=centroid1[0] && centroid1[0]<=other->x+other->w*factor &&
		other->y<=centroid1[1] && centroid1[1]<=other->y+other->h*factor){
			return true;
	}
/*	if( pow(
		( 	pow(fabs(centroid1[0]- centroid2[0]),2) +  
			pow(fabs(centroid1[1]- centroid2[1]),2)
		),0.5)< EPSILON){
		return true;
	}*/
	
	return false;
}

bool Cobject::overlap(Cobject* other){
	
	if(! (this->contains(other->x,other->y) || this->contains(other->x+other->w,other->y)
		|| this->contains(other->x,other->y+other->h) || this->contains(other->x+other->w,other->y+other->h)))
			return false;
	return true;
}

bool Cobject::contains(int x,int y){
	if(x>= this->x && x<= this->x + this->w)
		if(y>=this->y && y< this->y + this->h)
			return true;
	
	return false;
}
bool Cobject::contains(Cobject * other){
	if(this->contains(other->x,other->y) && this->contains(other->x+other->w,other->y)
		&& this->contains(other->x,other->y+other->h) && this->contains(other->x+other->w,other->y+other->h))
			return true;
	return false;
}


}

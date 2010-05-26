#include "Garbage.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>

namespace utils{

Garbage::~Garbage(){
	//~ if(this->mbr!=NULL){
		//~ delete this->mbr;
	//~ }
}
Garbage::Garbage(utils::MinimalBoundingRectangle * myRect)
{
    this->mbr=myRect;
}

Garbage::Garbage(utils::MinimalBoundingRectangle * mbr,std::vector<int> centroid){
	this->mbr=mbr;
	this->centroid=centroid;
}

std::vector<int> Garbage::getCentroid(){
	return this->centroid;
	
}

double Garbage::angleTo()
{
    return 0.0;
}

double Garbage::distanceTo()
{
    return 0.0;
}

utils::MinimalBoundingRectangle * Garbage::boundingBox()
{
	return this->mbr;
}

}

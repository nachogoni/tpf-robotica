#include "Garbage.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>

namespace vision{

Garbage::~Garbage(){
	//~ if(this->mbr!=NULL){
		//~ delete this->mbr;
	//~ }
}
Garbage::Garbage(vision::MinimalBoundingRectangle * myRect)
{
    this->mbr=myRect;
}

Garbage::Garbage(vision::MinimalBoundingRectangle * mbr,std::vector<int> centroid){
	this->mbr=mbr;
	this->centroid=centroid;
}

Garbage::Garbage(vision::MinimalBoundingRectangle * mbr,std::vector<int> centroid,Contours * contour){
	this->mbr=mbr;
	this->centroid=centroid;
	this->area=contour->getArea();
	this->perimeter=contour->getPerimeter();
}


std::vector<int> Garbage::getCentroid(){
	return this->centroid;
	
}

vision::MinimalBoundingRectangle * Garbage::boundingBox()
{
	return this->mbr;
}

}

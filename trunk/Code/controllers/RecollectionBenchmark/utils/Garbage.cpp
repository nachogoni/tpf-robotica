#include <utils/Garbage.h>
#include <stdlib.h>

namespace utils{

Garbage::Garbage(utils::MinimalBoundingRectangle * myRect)
{
    this->mbr=myRect;
}

utils::MinimalBoundingRectangle * Garbage::boundingBox()
{
	return this->mbr;
}

Garbage::~Garbage()
{
	delete this->mbr;
	this->mbr = NULL;
}

}

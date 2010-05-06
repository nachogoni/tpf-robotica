#include "Garbage.h"

namespace utils{

Garbage::Garbage(utils::MinimalBoundingRectangle * myRect)
{
    this->mbr=myRect;
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

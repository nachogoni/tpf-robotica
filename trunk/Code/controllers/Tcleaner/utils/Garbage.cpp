#include <utils/Garbage.h>

namespace utils{

Garbage::Garbage(utils::MinimalBoundingRectangle * myRect)
{
    this->mbr=myRect;
}

utils::MinimalBoundingRectangle * Garbage::boundingBox()
{
	return this->mbr;
}

}

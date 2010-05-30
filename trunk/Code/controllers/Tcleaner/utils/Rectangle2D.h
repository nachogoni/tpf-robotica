#ifndef utils_Rectangle2D_h
#define utils_Rectangle2D_h

#include <utils/MyPoint.h>

namespace utils{

class Rectangle2D {

	public:

		//TODO Refactor
		Rectangle2D(utils::MyPoint * p, double shortDistance, double longDistance, double orientation, double fovh);

		utils::MyPoint * getMinPoint();

		utils::MyPoint * getMaxPoint();

		bool containsPoint(utils::MyPoint * p);

		~Rectangle2D();

	private:
		bool triangleContainsPoint(utils::MyPoint * tFirstVector, utils::MyPoint * tSecondVector, utils::MyPoint * vectorTriangleToPoint, double uu, double uv, double vv);

		double min(double a1, double a2, double a3, double a4);

		utils::MyPoint * a1;
		utils::MyPoint * a2;
		utils::MyPoint * b1;
		utils::MyPoint * b2;
		utils::MyPoint * u;
		utils::MyPoint * v;
		utils::MyPoint * w;
		double uu;
		double uv;
		double vv;
		double uw;
		double ww;

};

}

#endif // utils_Rectangle2D_h

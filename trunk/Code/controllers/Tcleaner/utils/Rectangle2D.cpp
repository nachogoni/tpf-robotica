#include <utils/Rectangle2D.h>
#include <math.h>
#include <stdio.h>

namespace utils{

Rectangle2D::Rectangle2D(utils::MyPoint * p, double shortDistance, double longDistance, double orientation, double fovh){
	double senAlpha = sin(orientation);
	double cosAlpha = cos(orientation);
	double dSenAlpha = shortDistance * senAlpha;
	double dCosAlpha = shortDistance * cosAlpha;
	double dhSenAlpha = longDistance * senAlpha;
	double dhCosAlpha = longDistance * cosAlpha;
	double tanHalfFOV = tan(fovh/2);
	double dTanHalfFOV = shortDistance * tanHalfFOV;
	this->a1 = new utils::MyPoint(p->getX() + dSenAlpha + tanHalfFOV * ( - dCosAlpha) , p->getY() + dCosAlpha + tanHalfFOV * ( dSenAlpha));
	this->a2 = new utils::MyPoint(p->getX() + dSenAlpha - tanHalfFOV * ( - dCosAlpha) , p->getY() + dCosAlpha - tanHalfFOV * ( dSenAlpha));

	this->b1 = new utils::MyPoint(p->getX() + dhSenAlpha + dTanHalfFOV * ( - dCosAlpha) , p->getY() + dhCosAlpha + dTanHalfFOV * ( dSenAlpha));
	this->b2 = new utils::MyPoint(p->getX() + dhSenAlpha - dTanHalfFOV * ( - dCosAlpha) , p->getY() + dhCosAlpha - dTanHalfFOV * ( dSenAlpha));
	printf("A: (%g : %g)\nB: (%g : %g)\nC: (%g : %g)\nD: (%g : %g)\n",a2->getX(),a2->getY(),a1->getX(),a1->getY(),b1->getX(),b1->getY(),b2->getX(),b2->getY());
	printf("w: %g, h: %g\n",2*dTanHalfFOV, longDistance - shortDistance);
	this->u = this->a2->subNew(this->a1);
	this->v = this->b1->subNew(this->a1);
	this->w = this->b2->subNew(this->a1);
}

Rectangle2D::~Rectangle2D(){
	delete this->a1;
	delete this->a2;
	delete this->b1;
	delete this->b2;
	delete this->u;
	delete this->v;
	delete this->w;
}

utils::MyPoint * Rectangle2D::getMinPoint(){
	double minX = this->min(this->a1->getX(),this->a2->getX(),this->b1->getX(),this->b2->getX());
	double minZ = this->min(this->a1->getY(),this->a2->getY(),this->b1->getY(),this->b2->getY());
	return new utils::MyPoint(minX,minZ);
}

utils::MyPoint * Rectangle2D::getMaxPoint(){
	double maxX = this->max(this->a1->getX(),this->a2->getX(),this->b1->getX(),this->b2->getX());
	double maxZ = this->max(this->a1->getY(),this->a2->getY(),this->b1->getY(),this->b2->getY());
	return new utils::MyPoint(maxX,maxZ);
}

double Rectangle2D::min(double a1, double a2, double a3, double a4){
	if ( a1 < a2 && a1 < a3 && a1 < a4 )
		return a1;
	if ( a2 < a3 && a2 < a4 )
		return a2;
	if ( a3 < a4 )
		return a3;
	return a4;
}

double Rectangle2D::max(double a1, double a2, double a3, double a4){
	if ( a1 > a2 && a1 > a3 && a1 > a4 )
		return a1;
	if ( a2 > a3 && a2 > a4 )
		return a2;
	if ( a3 > a4 )
		return a3;
	return a4;
}

bool Rectangle2D::containsPoint(utils::MyPoint * p){
	return this->triangleContainsPoint(u,v,p->subNew(a1),uu,uv,vv) || 
			this->triangleContainsPoint(u,w,p->subNew(a1),uu,uw,ww);
}

bool Rectangle2D::triangleContainsPoint(utils::MyPoint * tFirstVector, utils::MyPoint * tSecondVector, utils::MyPoint * vectorTriangleToPoint, double uu, double uv, double vv){
	double awu = vectorTriangleToPoint->dot(tFirstVector);
	double awv = vectorTriangleToPoint->dot(tSecondVector);

	double D = uv * uv - uu * vv;

	double s = (uv * awv - vv * awu) / D;
	if (s < -RECT_INTERSECT_EPS || s > (1.0+RECT_INTERSECT_EPS) )        // I is outside T
		return false;
	double t = (uv * awu - uu * awv) / D;
	if (t < -RECT_INTERSECT_EPS || (s + t) > (1.0+RECT_INTERSECT_EPS) )  // I is outside T
		return false;
	
	return true;                      // I is in T
}

}

#include <utils/Rectangle2D.h>
#include <math.h>

namespace utils{

Rectangle2D::Rectangle2D(utils::MyPoint * p, double shortDistance, double longDistance, double orientation, double fovh){
	double senAlpha = sin(orientation);
	double cosAlpha = cos(orientation);
	double dSenAlpha = shortDistance * senAlpha;
	double dCosAlpha = shortDistance * cosAlpha;
	double dhSenAlpha = longDistance * senAlpha;
	double dhCosAlpha = longDistance * cosAlpha;
	double senHalfFOV = sin(fovh/2);
	double dSenHalfFOV = shortDistance * senHalfFOV;
	this->a1 = new utils::MyPoint(p->getX() + dSenAlpha + senHalfFOV * ( - dCosAlpha) , p->getZ() + dCosAlpha + senHalfFOV * ( dSenAlpha));
	this->a2 = new utils::MyPoint(p->getX() + dSenAlpha - senHalfFOV * ( - dCosAlpha) , p->getZ() + dCosAlpha - senHalfFOV * ( dSenAlpha));

	this->b1 = new utils::MyPoint(p->getX() + dhSenAlpha + dSenHalfFOV * ( - dCosAlpha) , p->getZ() + dhCosAlpha + dSenHalfFOV * ( dSenAlpha));
	this->b2 = new utils::MyPoint(p->getX() + dhSenAlpha - dSenHalfFOV * ( - dCosAlpha) , p->getZ() + dhCosAlpha - dSenHalfFOV * ( dSenAlpha));
	this->u = this->a2->subNew(this->a1);
	this->v = this->b1->subNew(this->a1);
	this->w = this->b2->subNew(this->a1);
}

Rectangle2D::~Rectangle2D(){
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
	double wu = vectorTriangleToPoint->dot(tFirstVector);
	double wv = vectorTriangleToPoint->dot(tSecondVector);

	double D = uv * uv - uu * vv;

	double s = (uv * wv - vv * wu) / D;
	if (s < 0.0 || s > 1.0)        // I is outside T
		return false;
	double t = (uv * wu - uu * wv) / D;
	if (t < 0.0 || (s + t) > 1.0)  // I is outside T
		return false;
	
	return true;                      // I is in T
}

}

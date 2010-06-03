#include <utils/Rectangle2D.h>
#include <math.h>
#include <stdio.h>
#include <utils/MyAngle.h>

namespace utils{

Rectangle2D::Rectangle2D(utils::MyPoint * p, double shortDistance, double longDistance, double orientation, double fov){
	printf("TODO: %g , %g , %g , %g , %g , %g\n",p->getX(),p->getY(),shortDistance,longDistance,orientation,fov);
	double angle = orientation + PI;

	double hfov = fov / 2.0;

	double an = tan(hfov) * shortDistance;

	double dp = sqrt( shortDistance * shortDistance + an * an );
	double dpp = sqrt( longDistance * longDistance + an * an );

	double Dangle = atan(an/longDistance);

	this->a = p->plusVector(dp,angle-hfov);
	this->b = p->plusVector(dp,angle+hfov);
	this->c = p->plusVector(dpp,angle+Dangle);
	this->d = p->plusVector(dpp,angle-Dangle);

	this->u = this->b->subNew(this->a);

	this->v = this->c->subNew(this->a);
	this->w = this->d->subNew(this->a);
	printf("A: (%g : %g)\nB: (%g : %g)\nC: (%g : %g)\nD: (%g : %g)\n",a->getX(),a->getY(),b->getX(),b->getY(),c->getX(),c->getY(),d->getX(),d->getY());

	printf("[%g,%g,%g,%g,%g]\n[%g,%g,%g,%g,%g]\n",a->getX(),b->getX(),c->getX(),d->getX(),p->getX(),a->getY(),b->getY(),c->getY(),d->getY(),p->getY());

	printf("w: %g, h: %g\n",2*an, longDistance - shortDistance);
}

Rectangle2D::~Rectangle2D(){
	delete this->a;
	delete this->b;
	delete this->c;
	delete this->d;
	delete this->u;
	delete this->v;
	delete this->w;
}

utils::MyPoint * Rectangle2D::getMinPoint(){
	double minX = this->min(this->a->getX(),this->b->getX(),this->c->getX(),this->d->getX());
	double minZ = this->min(this->a->getY(),this->b->getY(),this->c->getY(),this->d->getY());
	return new utils::MyPoint(minX,minZ);
}

utils::MyPoint * Rectangle2D::getMaxPoint(){
	double maxX = this->max(this->a->getX(),this->b->getX(),this->c->getX(),this->d->getX());
	double maxZ = this->max(this->a->getY(),this->b->getY(),this->c->getY(),this->d->getY());
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
	return this->triangleContainsPoint(u,v,p->subNew(this->a),uu,uv,vv) || 
			this->triangleContainsPoint(u,w,p->subNew(this->a),uu,uw,ww);
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

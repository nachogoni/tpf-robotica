// Class automatically generated by Dev-C++ New Class wizard

#include <utils/MyPoint.h> // class's header file

namespace utils {

// class constructor
MyPoint::MyPoint(double x, double y)
{
	this->x = x;
	this->y = y;
}

// class destructor
MyPoint::~MyPoint()
{
	// insert your code here
}

void MyPoint::add(double x, double y){
	this->x = this->x + x;
	this->y = this->y + y;
}

MyPoint * MyPoint::subNew(MyPoint * p){
	return new MyPoint( this->x - p->getX() , this->y - p->getY() );
}

void MyPoint::setX(double x){
	this->x = x;
}

void MyPoint::setY(double y){
	this->y = y;
}

double MyPoint::getX(){
	return this->x;
}

double MyPoint::getY(){
	return this->y;
}

double MyPoint::dot(MyPoint * p){
	return this->x * p->getX() + this->y * p->getY();
}


std::ostream& MyPoint::operator<<(std::ostream& out) {
  out << "(" << this->x << "," << this->y << ")";
  return out;
}


}
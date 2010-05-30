// Class automatically generated by Dev-C++ New Class wizard

#ifndef utils_MyPoint_h
#define utils_MyPoint_h
#include <iostream>

namespace utils {

/**
 * No description
 */
class MyPoint
{
	public:
		// class constructor
		MyPoint(double x, double y);
		// class destructor
		~MyPoint();
		
		double getX();
		double getY();
		void add(double x, double y);
		MyPoint * subNew(MyPoint * p);
		void setX(double x);
		void setY(double y);
		double dot(MyPoint * p);
		std::ostream& operator<<(std::ostream& out);
	private:
		double x;
		double y;

};

}
#endif // utils_MyPoint_h

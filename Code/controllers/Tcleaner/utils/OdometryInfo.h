// Class automatically generated by Dev-C++ New Class wizard

#ifndef utils_OdometryInfo_h
#define utils_OdometryInfo_h

#include <robotapi/IDifferentialWheels.h>
#include <utils/MyPoint.h>
#include <utils/MyAngle.h>

namespace utils{

/**
 * No description
 */
class OdometryInfo
{
	public:
		// class constructor
		OdometryInfo(robotapi::IDifferentialWheels * wheels, double distanceBetweenWheels, double wheelRadius, double encoderResolution, double initialX, double initialY, double initialTheta);

		// class destructor
		~OdometryInfo();

        void computeOdometry();
		double getOrientation();
		utils::MyPoint * getPosition();
	private:
        void computePosition(double ldist, double rdist);
        double distanceBetweenWheels;
		double wheelRadius;
		double encoderResolution;
		double lastLeftEncoder;
		double lastRightEncoder;
		robotapi::IDifferentialWheels * wheels;
		utils::MyPoint * lastPosition;
		utils::MyAngle * lastAngle;
};

}

#endif // ODOMETRYINFO_H
#ifndef robotapi_DifferentialWheelsWOdometry_h
#define robotapi_DifferentialWheelsWOdometry_h

#include <robotapi/IDifferentialWheels.h>
#include <utils/MyPoint.h>
#include <utils/MyAngle.h>

namespace robotapi {

class DifferentialWheelsWOdometry : public robotapi::IDifferentialWheels {

		public:

    		DifferentialWheelsWOdometry(double distanceBetweenWheels, double wheelRadius,
										double encoderResolution, double initialX,
										double initialY, double initialTheta);

			// class destructor
			virtual ~DifferentialWheelsWOdometry(){};

			virtual double getLeftEncoder();

			virtual double getRightEncoder();

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

} /* End of namespace robotapi */

#endif // robotapi_DifferentialWheelsWOdometry_h

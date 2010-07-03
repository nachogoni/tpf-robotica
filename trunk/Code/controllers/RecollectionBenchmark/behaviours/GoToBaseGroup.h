// Class automatically generated by Dev-C++ New Class wizard

#ifndef behaviours_GoToBaseGroup_H
#define behaviours_GoToBaseGroup_H

#include "AbstractBehaviour.h"
#include "WorldInfo.h"
#include <list>
#include <behaviours/GoToBase.h>
#include <behaviours/FindLine.h>
#include <behaviours/PositionInLine.h>
#include <behaviours/Recharge.h>

#define GO_TO_BASE_ORIENTATION_TOLE (PI/4)
#define GO_TO_BASE_BASE_FIX_ORIENTATION_TOLE 0.3


#define GO_TO_BASE_BASE_X -0.874047
#define GO_TO_BASE_TOLE 0.2
#define GO_TO_BASE_BASE_POSITION (GO_TO_BASE_BASE_X+GO_TO_BASE_TOLE)

#define GO_TO_BASE_PASSAGE_BEGIN_X -0.6
#define GO_TO_BASE_PASSAGE_LINE_Z 0.100174

#define GO_TO_BASE_LINE_MARK_X -0.626685

#define GO_TO_BASE_LINE_ORIENTATION_CORRECTION (3.142375307)

// FROM Z TO 1
#define GO_TO_BASE_FROM_Z_LEFT (0.382857)

#define GO_TO_BASE_X_CORRECTION_LINE (-0.447202)

// FROM Z TO -1
#define GO_TO_BASE_FROM_Z_RIGHT (-0.207086)

#define GO_TO_BASE_EMPTY_BIAS 5500
#define GO_TO_BASE_FULL_BIAS 15000

namespace behaviours {

/**
 * No description
 */
class GoToBaseGroup : public AbstractBehaviour {

	public:
		GoToBaseGroup(WorldInfo * wi, robotapi::IRobot * robot, robotapi::IBattery * robotBatt, robotapi::IBattery * pcBatt, robotapi::IDifferentialWheels * wheels, std::vector<robotapi::IDistanceSensor*> & fss);

		~GoToBaseGroup();

    	void sense();

	    void action();

	private:
		bool onMark();

		bool inLine();

		bool inPosition();

		void correctOrientation();

	    behaviours::AbstractBehaviour * myBehaviours [4];
		robotapi::IDifferentialWheels * wheels;
	    robotapi::IBattery * robotBattery;
		robotapi::IBattery * pcBattery;
		std::vector<robotapi::IDistanceSensor*> * fss;
		WorldInfo * wi;
};

}

#endif // GOTOBASEGROUP_H
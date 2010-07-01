#include "GoToBase.h"
#include "GoToBaseGroup.h"
#include "GarbageCleaner.h"

namespace behaviours {

	GoToBase::GoToBase(robotapi::IDifferentialWheels * wheels, std::vector<robotapi::IDistanceSensor*> & fss) : AbstractBehaviour("Go To Base"){
		this->wheels = wheels;
		this->fss = &fss;
	}

	void GoToBase::sense(){

	}

    void GoToBase::action(){

		double lspd = GO_TO_BASE_BASE_SPD;
		double rspd = GO_TO_BASE_BASE_SPD;
		
		// Left sensor is on line?
		if ( (*this->fss).at(0)->getValue() < LINE_THRESHOLD ){
			lspd = lspd * ( 1 - GO_TO_BASE_SPD_FACTOR );
			rspd = rspd * ( 1 + GO_TO_BASE_SPD_FACTOR );
		}
		// Right sensor is on line?
		if ( (*this->fss).at(2)->getValue() < LINE_THRESHOLD ){
			lspd = lspd * ( 1 + GO_TO_BASE_SPD_FACTOR );
			rspd = rspd * ( 1 - GO_TO_BASE_SPD_FACTOR );
		}
		this->wheels->setSpeed(lspd,rspd);
	}

} /* End of namespace behaviours */

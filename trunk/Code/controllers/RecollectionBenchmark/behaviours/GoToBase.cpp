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

		double lspd = GET_PARAM(GO_TO_BASE_BASE_SPD);
		double rspd = GET_PARAM(GO_TO_BASE_BASE_SPD);
		
		// Left sensor is on line?
		if ( this->inLine((*this->fss).at(0)->getValue()) ){
			lspd = lspd * ( 1 - GET_PARAM(GO_TO_BASE_SPD_FACTOR) );
			rspd = rspd * ( 1 + GET_PARAM(GO_TO_BASE_SPD_FACTOR) );
		}
		// Right sensor is on line?
		if ( this->inLine((*this->fss).at(2)->getValue()) ){
			lspd = lspd * ( 1 + GET_PARAM(GO_TO_BASE_SPD_FACTOR) );
			rspd = rspd * ( 1 - GET_PARAM(GO_TO_BASE_SPD_FACTOR) );
		}
		this->wheels->setSpeed(lspd,rspd);
	}

} /* End of namespace behaviours */

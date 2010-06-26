#include "GoToBase.h"
#include "GoToBaseGroup.h"
#include "GarbageCleaner.h"

#define BASE_SPD 100.0
#define SPD_FACTOR 0.7

namespace behaviours {

	GoToBase::GoToBase(robotapi::IDifferentialWheels * wheels, std::vector<robotapi::IDistanceSensor*> & fss) : AbstractBehaviour("Go To Base"){
		this->wheels = wheels;
		this->fss = &fss;
	}

	void GoToBase::sense(){

	}

    void GoToBase::action(){
		double lspd = BASE_SPD;
		double rspd = BASE_SPD;
		
		// Left sensor is on line?
		if ( (*this->fss).at(0)->getValue() < LINE_THRESHOLD ){
			lspd = lspd * ( 1 - SPD_FACTOR );
			rspd = rspd * ( 1 + SPD_FACTOR );
		}
		// Right sensor is on line?
		if ( (*this->fss).at(2)->getValue() < LINE_THRESHOLD ){
			lspd = lspd * ( 1 + SPD_FACTOR );
			rspd = rspd * ( 1 - SPD_FACTOR );
		}
		this->wheels->setSpeed(lspd,rspd);
	}

} /* End of namespace behaviours */

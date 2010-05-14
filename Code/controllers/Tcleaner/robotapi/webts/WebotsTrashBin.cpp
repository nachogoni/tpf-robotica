#include "WebotsTrashBin.h"
#include <math.h>
#include <stdio.h>

namespace robotapi {
namespace webts {

    WebotsTrashBin::WebotsTrashBin( webots::TouchSensor & ts ) : WebotsDevice ( ts ){
		this->myts = &ts;
		this->currentValue = 0;
	}

	int WebotsTrashBin::enable(int ms){
		this->myts->enable(ms);
		return 0;
	}

	int WebotsTrashBin::disable(){
		this->myts->disable();
		this->currentValue = 0;
		return 0;
	}

	int WebotsTrashBin::getValue(){
		if ( this->currentValue >= 1 )
		    return this->currentValue;
        this->currentValue = this->myts->getValue();
		return (int)ceil(this->currentValue);
	}

	bool WebotsTrashBin::isFull(){
		return this->getValue() >= this->fullBias;
	}

	void WebotsTrashBin::setFullBias(double bias){
		if ( bias < 0 )
			return;

		this->fullBias = bias;
	}

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */

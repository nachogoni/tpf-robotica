#include "WebotsTrashBin.h"
#include <math.h>

namespace robotapi {
namespace webts {

    WebotsTrashBin::WebotsTrashBin( webots::TouchSensor & ts ) : WebotsDevice ( ts ){
		this->myts = &ts;
	}

	int WebotsTrashBin::enable(int ms){
		this->myts->enable(ms);
		return 0;
	}

	int WebotsTrashBin::disable(){
		this->myts->disable();
		return 0;
	}

	int WebotsTrashBin::getValue(){
		return ceil(this->myts->getValue());
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

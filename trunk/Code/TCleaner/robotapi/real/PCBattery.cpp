#include "PCBattery.h"

namespace robotapi {
namespace real {

	PCBattery::PCBattery(std::string name) : RealDevice(&name){
	}

	void PCBattery::enable(int ms){
		return;
	}

	void PCBattery::disable(){
		return;
	}

	double PCBattery::getValue(){
		return 0.2;
	}

	bool PCBattery::isFull(){
		return false;
	}

	void PCBattery::setEmptyBias(double bias){
	}

	bool PCBattery::isEmpty(){
		return false;
	}

	void PCBattery::setFullBias(double bias){
	}

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */

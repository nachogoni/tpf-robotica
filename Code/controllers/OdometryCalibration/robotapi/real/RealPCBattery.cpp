#include "RealPCBattery.h"

namespace robotapi {
namespace real {

	RealPCBattery::RealPCBattery(std::string name) : RealDevice(&name){
	}

	void RealPCBattery::enable(int ms){
		return;
	}

	void RealPCBattery::disable(){
		return;
	}

	double RealPCBattery::getValue(){
		return 0.2;
	}

	bool RealPCBattery::isFull(){
		return false;
	}

	void RealPCBattery::setEmptyBias(double bias){
	}

	bool RealPCBattery::isEmpty(){
		return false;
	}

	void RealPCBattery::setFullBias(double bias){
	}

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */

#include "RealBattery.h"

namespace robotapi {
namespace real {

	RealBattery::RealBattery(protocol::handlers::BatteryBoardPacketHandler * bbph){
		this->bbph = bbph;
	}

	void RealBattery::enable(int ms){
		return;
	}

	void RealBattery::disable(){
		return;
	}

	double RealBattery::getValue(){
		return this->bbph->getValue();
	}

	bool RealBattery::isFull(){
		return this->bbph->isFull();
	}

	void RealBattery::setEmptyBias(double bias){
		this->bbph->setEmptyBias(bias);
	}

	bool RealBattery::isEmpty(){
		return this->bbph->isEmpty();
	}

	void RealBattery::setFullBias(double bias){
		this->bbph->setFullBias(bias);
	}

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */

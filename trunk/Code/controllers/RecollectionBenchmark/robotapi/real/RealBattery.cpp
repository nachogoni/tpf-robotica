#include "RealBattery.h"

namespace robotapi {
namespace real {

	RealBattery::RealBattery(protocol::handlers::BatteryBoardPacketHandler * bbph,std::string name, int sensorNumber)
							: RealDevice(&name){
		this->bbph = bbph;
		this->id = sensorNumber;
	}

	void RealBattery::enable(int ms){
		this->bbph->enable(this->id);
		return;
	}

	void RealBattery::disable(){
		this->bbph->disable(this->id);
		return;
	}

	double RealBattery::getValue(){
		return this->bbph->getValue(this->id);
	}

	bool RealBattery::isFull(){
		return this->bbph->isFull(this->id);
	}

	void RealBattery::setEmptyBias(double bias){
		this->bbph->setEmptyBias(this->id, bias);
	}

	bool RealBattery::isEmpty(){
		return this->bbph->isEmpty(this->id);
	}

	void RealBattery::setFullBias(double bias){
		this->bbph->setFullBias(this->id, bias);
	}

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */

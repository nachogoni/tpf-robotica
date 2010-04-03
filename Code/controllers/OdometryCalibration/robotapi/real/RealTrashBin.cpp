#include "RealTrashBin.h"

namespace robotapi {
namespace real {

RealTrashBin::RealTrashBin(protocol::handlers::TrashBinBoardPacketHandler * tbbph,std::string name)
							: RealDevice(&name){
	this->boardHandler = tbbph;
}

int RealTrashBin::enable(int ms){
	return 0;
}

int RealTrashBin::disable(){
	return 0;
}

int RealTrashBin::getValue(){
	return this->boardHandler->getValue();
}

bool RealTrashBin::isFull(){
	return this->boardHandler->isFull();
}

void RealTrashBin::setFullBias(double bias){
	return this->boardHandler->setFullBias(bias);
}

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */

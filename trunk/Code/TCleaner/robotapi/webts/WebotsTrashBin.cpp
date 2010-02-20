#include "WebotsTrashBin.h"

namespace robotapi {
namespace webts {

    WebotsTrashBin::WebotsTrashBin(std::string & name, IWbDeviceTag tag) : WebotsDevice ( name, tag ){
	}

	int WebotsTrashBin::enable(int ms){
		return 0;
	}

	int WebotsTrashBin::disable(){
		return 0;
	}

	int WebotsTrashBin::getValue(){
		return 0;
	}

	bool WebotsTrashBin::isFull(){
		return false;
	}

	void WebotsTrashBin::setFullBias(double bias){
		return;
	}

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */

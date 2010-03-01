#include "WebotsDeviceTag.h"

namespace robotapi {
namespace webts {

	WbDeviceTag mytag;

    WebotsDeviceTag::WebotsDeviceTag(WbDeviceTag tag){
		mytag = tag;
	}

    WebotsDeviceTag::WebotsDeviceTag(int tag){
        dtag = tag;
    }

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */

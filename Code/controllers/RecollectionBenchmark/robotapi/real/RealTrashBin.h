#ifndef robotapi_real_RealTrashBin_h
#define robotapi_real_RealTrashBin_h

#include <robotapi/ITrashBin.h>
#include <protocol/handlers/TrashBinBoardPacketHandler.h>
#include <robotapi/real/RealDevice.h>

namespace robotapi {
namespace real {

class RealTrashBin : virtual public robotapi::ITrashBin , public robotapi::real::RealDevice {

	public:
		RealTrashBin(protocol::handlers::TrashBinBoardPacketHandler * tbbph, std::string name);

		int enable(int ms);

		int disable();

		int getValue();

		bool isFull();

		void setFullBias(double bias);

	private:
		protocol::handlers::TrashBinBoardPacketHandler * boardHandler;

	};

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */

#endif // robotapi_real_RealTrashBin_h

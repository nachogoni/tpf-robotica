#ifndef robotapi_real_RealTrashBin_h
#define robotapi_real_RealTrashBin_h

#include <robotapi/ITrashBin.h>
#include <protocol/handlers/TrashBinBoardPacketHandler.h>

namespace robotapi {
namespace real {

class RealTrashBin : virtual public robotapi::ITrashBin {

	public:
		RealTrashBin::RealTrashBin(protocol::handlers::TrashBinBoardPacketHandler * tbbph);

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

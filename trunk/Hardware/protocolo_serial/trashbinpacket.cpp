// Class automatically generated by Dev-C++ New Class wizard

#include "trashbinpacket.h" // class's header file

// class constructor
TrashBinPacket::TrashBinPacket() : GroupPacket(TRASHBIN_ID)
{
}

// class destructor
TrashBinPacket::~TrashBinPacket()
{
	// insert your code here
}

void TrashBinPacket::senseTrashBin(){
	this->setCommand(CMD_TRASHBIN_VALUE);
}

bool TrashBinPacket::isTrashBinFull(){
	return this->getCommand() == CMD_FULL_TRASHBIN_ALARM;
}

short TrashBinPacket::getTrashBinValue(){
	return this->getShortData();
}

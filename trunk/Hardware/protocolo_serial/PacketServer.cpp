// Class automatically generated by Dev-C++ New Class wizard

#include "PacketServer.h" // class's header file

// class constructor
PacketServer::PacketServer()
{
	// insert your code here
}

// class destructor
PacketServer::~PacketServer()
{
	// insert your code here
}

void PacketServer::sendPacket(Packet * p){
	// Apply mutex
	
	this->toSend.push(p);
	
	// Release mutex
}

void PacketServer::sendAPacket(Packet * p){
	unsigned char length = (unsigned char) p->getActualLength();
	unsigned char i;
//	char * packet = p->getPacket();
	
	for ( i = 0 ; i < length ; i++ ){
	    // Write in fd
	    // write(fd,packet[i]);
	    i = i;
	}
	this->waitingForResponse.push_back(p);
}

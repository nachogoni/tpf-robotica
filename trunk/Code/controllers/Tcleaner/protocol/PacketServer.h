// Class automatically generated by Dev-C++ New Class wizard

#ifndef PACKETSERVER_H
#define PACKETSERVER_H

#include <queue>
#include <list>
#include <map>
#include <protocol/Packet.h>
#include <protocol/BoardPacketHandler.h>

#define SERIALDEVICE "/dev/ttyUSB0"
#define MAX(a,b) (a>b?a:b)
#define PIPE_IN 0
#define PIPE_OUT 1


//#ifdef __linux__
//#include <mutex>
//#include <thread>
#include <cc++/thread.h>

//#endif

namespace protocol {

/**
 * No description
 */
class PacketServer : public Thread {
	public:
		// class constructor
		PacketServer();
		// class destructor
		~PacketServer();
		
		void sendPacket(Packet * p);
		void registerHandler(BoardPacketHandler * bph,int groupid,int boardid);
	private:
		bool init();
		void sendAPacket(Packet * p);
		std::queue<Packet *> toSend;
		std::list<Packet *> waitingForResponse;
		std::map<char, std::map<char, BoardPacketHandler *> > handlers;
		void run(void);

		#ifdef __linux__
		int pipes[2];
		int serfd;
		Mutex toSendMutex;
		#endif
		
};

}

#endif // PACKETSERVER_H
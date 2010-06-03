g++ -g -o controller realTcleaner.cpp GarbageCleaner.cpp WorldInfo.cpp \
			protocol/packet.cpp protocol/PacketServer.cpp protocol/BoardPacketHandler.cpp \
			behaviours/*.cpp utils/*.cpp robotapi/*.cpp robotapi/real/*.cpp \
			protocol/packets/*.cpp protocol/handlers/*.cpp -I. -lccext2 `pkg-config opencv --libs` `pkg-config opencv --cflags` -std=c++0x

#
# Makefile for a Webots C++ controller
#
# if your controller uses several C++ sources files:
CPP_SOURCES = tcleaner.cpp GarbageCleaner.cpp behaviours/AbstractBehaviour.cpp behaviours/AvoidObstacle.cpp behaviours/CollectGarbage.cpp behaviours/FindLine.cpp behaviours/FocusGarbage.cpp behaviours/GoToBase.cpp behaviours/GoToDisposal.cpp behaviours/Recharge.cpp behaviours/UnloadGarbage.cpp behaviours/Wander.cpp robotapi/real/RealBattery.cpp robotapi/real/RealCamera.cpp robotapi/real/RealDevice.cpp robotapi/real/RealDeviceTag.cpp robotapi/real/RealDifferentialWheels.cpp robotapi/real/RealDistanceSensor.cpp robotapi/real/RealImage.cpp robotapi/real/RealRobot.cpp robotapi/real/RealServo.cpp utils/GarbageRecognition.cpp utils/Rectangle.cpp robotapi/webts/WebotsBattery.cpp robotapi/webts/WebotsCamera.cpp robotapi/webts/WebotsDevice.cpp robotapi/webts/WebotsDeviceTag.cpp robotapi/webts/WebotsDifferentialWheels.cpp robotapi/webts/WebotsDistanceSensor.cpp robotapi/webts/WebotsImage.cpp robotapi/webts/WebotsRobot.cpp robotapi/webts/WebotsServo.cpp robotapi/real/RealTrashBin.cpp robotapi/webts/WebotsTrashBin.cpp protocol/BoardPacketHandler.cpp protocol/packet.cpp protocol/PacketServer.cpp protocol/packets/batterypacket.cpp protocol/packets/BoardPacket.cpp protocol/packets/DCMotorPacket.cpp protocol/packets/DistanceSensorPacket.cpp protocol/packets/floorsensorpacket.cpp protocol/packets/GroupPacket.cpp protocol/packets/ServoMotorPacket.cpp protocol/packets/trashbinpacket.cpp protocol/packets/ultrasonicsensorpacket.cpp protocol/handlers/BatteryBoardPacketHandler.cpp protocol/handlers/DCMotorBoardPacketHandler.cpp protocol/handlers/DistanceSensorBoardPacketHandler.cpp protocol/handlers/ServoBoardPacketHandler.cpp protocol/handlers/TrashBinBoardPacketHandler.cpp robotapi/real/RealPCBattery.cpp robotapi/webts/WebotsPCBattery.cpp
# or
# CC_SOURCES = my_controller.cc my_clever_algo.cc my_gui.cc
#
# if your controller needs additional libraries,add them to the
# existing LIBRARIES variable:
# LIBRARIES=-lCppController -L/path/to/my/library -lmy_library -lmy_other_library
#
# if some special CFLAGS need to be passed, for example to find include
# files, or set optimization level:
CFLAGS= -I"C:/projects/tpf-robotica/Code/controllers/TCleaner"
#
# you may want, for example, to use the C API in your C++ program by adding:
# CFLAGS=-I"$(WEBOTS_HOME)/include/controller/c"

ifndef WEBOTS_HOME_PATH
ifeq ($(OS),Windows_NT)
nullstring :=
space := $(nullstring) # a string containing a single space
WEBOTS_HOME_PATH=$(subst $(space),\ ,$(strip $(subst \,/,$(WEBOTS_HOME))))
else
WEBOTS_HOME_PATH=$(WEBOTS_HOME)
endif
endif

include $(WEBOTS_HOME_PATH)/projects/default/controllers/Makefile.include

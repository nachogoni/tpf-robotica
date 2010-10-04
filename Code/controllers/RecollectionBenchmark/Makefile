#
# Makefile for a Webots C++ controller
#
# if your controller uses several C++ sources files:
CPP_SOURCES = recollectionbenchmark.cpp \
							WorldInfo.cpp GarbageCleaner.cpp \
							behaviours/AbstractBehaviour.cpp behaviours/AvoidObstacle.cpp \
							behaviours/CollectGarbage.cpp behaviours/GoToGarbage.cpp behaviours/FocusGarbage.cpp \
							behaviours/FindLine.cpp behaviours/GoToBase.cpp behaviours/Recharge.cpp \
							behaviours/GoToBaseGroup.cpp behaviours/PositionInLine.cpp behaviours/Recalibrate.cpp \
							behaviours/RemoveFromStuck.cpp behaviours/GoToDisposal.cpp behaviours/UnloadGarbage.cpp \
							behaviours/Wander.cpp behaviours/BehavioursParameters.cpp \
							vision/MinimalBoundingRectangle.cpp vision/Garbage.cpp vision/GarbageRecognition.cpp \
							vision/Contours.cpp vision/Histogram.cpp vision/Polygon.cpp vision/GarbageHistoric.cpp \
							vision/IGarbageRecognition.cpp vision/Windowing.cpp vision/Prediction.cpp \
							vision/GarbageRecognitionWithPrediction.cpp \
							utils/MyLine.cpp utils/MyPoint.cpp utils/MyAngle.cpp utils/MyWall.cpp \
							utils/ArenaGrid.cpp utils/ArenaGridSlot.cpp utils/Rectangle2D.cpp \
							robotapi/DifferentialWheelsWOdometry.cpp \
							robotapi/webts/WebotsBattery.cpp robotapi/webts/WebotsTrashBin.cpp robotapi/webts/WebotsPCBattery.cpp \
							robotapi/webts/WebotsCamera.cpp robotapi/webts/WebotsDevice.cpp robotapi/webts/WebotsDeviceTag.cpp \
							robotapi/webts/WebotsDifferentialWheels.cpp robotapi/webts/WebotsDistanceSensor.cpp \
							robotapi/webts/WebotsImage.cpp robotapi/webts/WebotsRobot.cpp robotapi/webts/WebotsServo.cpp \
							


# or
# CC_SOURCES = my_controller.cc my_clever_algo.cc my_gui.cc
#
# if your controller needs additional libraries,add them to the
# existing LIBRARIES variable:
#LIBRARIES= -lCppController  -L"C:/Program Files/OpenCV/lib" -L"C:/Program Files/OpenCV/otherlibs/highgui" -lcxcore -lcv -lcvaux -lhighgui -lml -g3
#LIBRARIES= -lCppController -lcv200 -lhighgui200 -lcxcore200 -L"C:/OpenCV2.0/lib"

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

CVLIBFLAGSCOMMAND = pkg-config opencv --libs

CVHEADFLAGSCOMMAND = pkg-config opencv --cflags

ifeq ($(OSTYPE),linux)
#LIBRARIES= -lCppController -lcv -lhighgui -lcxcore -L"$(HOME)/OpenCV-2.0.0/lib" -lccext2
OPENCVCFLAGS = $(shell $(CVHEADFLAGSCOMMAND) )
LIBRARIES= -lCppController -lccext2 $(OPENCVLFLAGS)
else
#LIBRARIES= -lCppController -lcv200 -lhighgui200 -lcxcore200 -L"C:/OpenCV2.0/lib"
LIBRARIES= -lCppController -lcv210 -lhighgui210 -lcxcore210 -L"C:/Archivos de programa/OpenCV/lib"
endif

#
# if some special CFLAGS need to be passed, for example to find include
# files, or set optimization level:

#CFLAGS= -g -Wall -I"C:/tpf-robotica/Code/controllers/TCleaner" -I"C:\OpenCV2.0\include\opencv"


ifeq ($(OSTYPE),linux)
OPENCVLFLAGS = $(shell $(CVLIBFLAGSCOMMAND) )
CFLAGS= -g -Wall -I"$(HOME)/tpf-robotica/Code/controllers/RecollectionBenchmark" $(OPENCVCFLAGS) -std=c++0x
else
#CFLAGS= -g -Wall -I"C:\projects\tpf-robotica\Code\controllers\RecollectionBenchmark" -I"C:/OpenCV2.0/include/opencv" 
CFLAGS= -g -Wall -I"C:\projects\tpf-robotica\Code\controllers\RecollectionBenchmark" -I"C:/Archivos de programa/OpenCV/include/opencv"
endif

#-L"C:/Program Files/OpenCV/lib" -lcxcore -lcv -lcvaux -lhighgui -lml -g3
#-L"C:/Program Files/OpenCV/lib" 
# you may want, for example, to use the C API in your C++ program by adding:
# CFLAGS=-I"$(WEBOTS_HOME)/include/controller/c"


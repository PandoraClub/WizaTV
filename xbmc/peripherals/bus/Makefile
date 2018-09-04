SRCS  = PeripheralBus.cpp

ifeq (0,1)
SRCS += linux/PeripheralBusUSBLibUdev.cpp
endif

ifeq (0,1)
SRCS += linux/PeripheralBusUSBLibUSB.cpp
endif

ifeq ($(findstring osx,arm-osx),osx)
SRCS += osx/PeripheralBusUSB.cpp
endif

ifeq (0,1)
SRCS += virtual/PeripheralBusCEC.cpp
endif

LIB = peripheral-bus.a

include /Users/osx/Desktop/kodi/xbmc/Makefile.include
-include $(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(SRCS)))

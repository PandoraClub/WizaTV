SRCS  = Peripheral.cpp
SRCS += PeripheralBluetooth.cpp
SRCS += PeripheralDisk.cpp
SRCS += PeripheralHID.cpp
SRCS += PeripheralImon.cpp
SRCS += PeripheralNIC.cpp
SRCS += PeripheralNyxboard.cpp
SRCS += PeripheralTuner.cpp

ifeq (0,1)
SRCS += PeripheralCecAdapter.cpp
endif

LIB = peripheral-devices.a

include /Users/osx/Desktop/kodi/xbmc/Makefile.include
-include $(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(SRCS)))

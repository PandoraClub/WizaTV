INCLUDES  = -I/Users/osx/Desktop/kodi/xbmc
INCLUDES += -I/Users/osx/Desktop/kodi/xbmc/xbmc
INCLUDES += -I/Users/osx/Desktop/kodi/xbmc/xbmc/linux

.SUFFIXES : .m .mm

SRCS  = XBMCHelper.m
SRCS += AutoPool.mm
SRCS += DarwinUtils.mm
SRCS += CocoaInterface.mm
SRCS += HotKeyController.m
SRCS += OSXGNUReplacements.c
SRCS += OSXTextInputResponder.mm
ifeq ($(findstring osx,arm-osx), osx)
SRCS += smc.c
endif
	
LIB = osx.a

include /Users/osx/Desktop/kodi/xbmc/Makefile.include

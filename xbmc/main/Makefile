.SUFFIXES : .m .mm .cpp

SRCS  = main.cpp
SRCS += posix/MessagePrinter.cpp

ifeq ($(findstring osx,arm-osx),osx)
SRCS += osx/SDLMain.mm
endif

LIB=main.a

include /Users/osx/Desktop/kodi/xbmc/Makefile.include
-include $(patsubst %.mm,%.P,$(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(SRCS))))

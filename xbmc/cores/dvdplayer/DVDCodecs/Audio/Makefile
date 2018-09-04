INCLUDES+=-I/Users/osx/Desktop/kodi/xbmc/xbmc/cores/dvdplayer

CXXFLAGS+=-DHAVE_MMX

SRCS  = DVDAudioCodecFFmpeg.cpp
SRCS += DVDAudioCodecPassthrough.cpp

LIB=Audio.a

include /Users/osx/Desktop/kodi/xbmc/Makefile.include
-include $(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(SRCS)))


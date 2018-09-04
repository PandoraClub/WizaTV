INCLUDES+=-I/Users/osx/Desktop/kodi/xbmc/xbmc/cores/dvdplayer

SRCS  = DVDDemux.cpp
SRCS += DVDDemuxBXA.cpp
SRCS += DVDDemuxCDDA.cpp
SRCS += DVDDemuxFFmpeg.cpp
SRCS += DVDDemuxPVRClient.cpp
SRCS += DVDDemuxShoutcast.cpp
SRCS += DVDDemuxUtils.cpp
SRCS += DVDDemuxVobsub.cpp
SRCS += DVDDemuxCC.cpp
SRCS += DVDFactoryDemuxer.cpp

LIB = DVDDemuxers.a

include /Users/osx/Desktop/kodi/xbmc/Makefile.include
-include $(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(SRCS)))


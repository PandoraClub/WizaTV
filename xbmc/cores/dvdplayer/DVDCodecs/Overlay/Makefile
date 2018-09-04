INCLUDES+=-I/Users/osx/Desktop/kodi/xbmc/xbmc/cores/dvdplayer

SRCS  = DVDOverlayCodec.cpp
SRCS += DVDOverlayCodecFFmpeg.cpp
SRCS += DVDOverlayCodecSSA.cpp
SRCS += DVDOverlayCodecText.cpp
SRCS += DVDOverlayCodecTX3G.cpp
SRCS += contrib/cc_decoder.c
SRCS += contrib/cc_decoder708.cpp

LIB = Overlay.a

include /Users/osx/Desktop/kodi/xbmc/Makefile.include
-include $(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(SRCS)))


CXXFLAGS += -D__STDC_FORMAT_MACROS

SRCS  = OMXHelper.cpp
SRCS += OMXAudio.cpp
SRCS += OMXVideo.cpp
SRCS += OMXAudioCodecOMX.cpp
SRCS += OMXPlayerAudio.cpp
SRCS += OMXPlayerVideo.cpp
SRCS += OMXImage.cpp

LIB = omxplayer.a

/Users/osx/Desktop/kodi/xbmc/system/advancedsettings.xml: $(LIB)
	cp -f omxplayer_advancedsettings.xml $@

include /Users/osx/Desktop/kodi/xbmc/Makefile.include
-include $(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(SRCS)))


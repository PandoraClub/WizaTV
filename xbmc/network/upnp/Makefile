ifeq (1, 1)
INCLUDES+=-I/Users/osx/Desktop/kodi/xbmc/lib/libUPnP \
          -I/Users/osx/Desktop/kodi/xbmc/lib/libUPnP/Platinum/Source/Core \
          -I/Users/osx/Desktop/kodi/xbmc/lib/libUPnP/Platinum/Source/Platinum \
          -I/Users/osx/Desktop/kodi/xbmc/lib/libUPnP/Platinum/Source/Devices/MediaConnect \
          -I/Users/osx/Desktop/kodi/xbmc/lib/libUPnP/Platinum/Source/Devices/MediaRenderer \
          -I/Users/osx/Desktop/kodi/xbmc/lib/libUPnP/Platinum/Source/Devices/MediaServer \
          -I/Users/osx/Desktop/kodi/xbmc/lib/libUPnP/Platinum/Source/Extras \
          -I/Users/osx/Desktop/kodi/xbmc/lib/libUPnP/Neptune/Source/System/Posix \
          -I/Users/osx/Desktop/kodi/xbmc/lib/libUPnP/Neptune/Source/Core

SRCS= UPnP.cpp \
      UPnPInternal.cpp \
      UPnPPlayer.cpp \
      UPnPRenderer.cpp \
      UPnPServer.cpp \
      UPnPSettings.cpp \

LIB=upnp.a

include /Users/osx/Desktop/kodi/xbmc/Makefile.include
-include $(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(SRCS)))

endif


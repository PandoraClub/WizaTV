SRCS  = cddb.cpp \
        DNSNameCache.cpp \
        EventClient.cpp \
        EventPacket.cpp \
        EventServer.cpp \
        GUIDialogAccessPoints.cpp \
        GUIDialogNetworkSetup.cpp \
        Network.cpp \
        NetworkServices.cpp \
        Socket.cpp \
        TCPServer.cpp \
        UdpClient.cpp \
        WakeOnAccess.cpp \
        WebServer.cpp \
        ZeroconfBrowser.cpp \
        Zeroconf.cpp \

ifeq (1, 1)
SRCS += AirPlayServer.cpp
endif

ifeq (1, 1)
SRCS += AirTunesServer.cpp
endif

LIB = network.a

include /Users/osx/Desktop/kodi/xbmc/Makefile.include
-include $(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(SRCS)))

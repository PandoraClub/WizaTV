ARCH=arm-osx
INCLUDES=-I. -I../../../xbmc/addons/include -I../../../xbmc -I../../../xbmc/cores/dvdplayer/DVDDemuxers
DEFINES+=
CXXFLAGS=-fPIC
LIBNAME=libXBMC_pvr
OBJS=$(LIBNAME).o

LIB_SHARED=../../../addons/library.xbmc.pvr/$(LIBNAME)-$(ARCH).so

all: $(LIB_SHARED)

$(LIB_SHARED): $(OBJS)
ifeq ($(findstring osx,$(ARCH)), osx)
	$(CXX) $(LDFLAGS) -Wl,-alias_list,/Users/osx/Desktop/kodi/xbmc/xbmc/cores/DllLoader/exports/wrapper_mach_alias \
	-bundle -undefined dynamic_lookup -o $@ \
	/Users/osx/Desktop/kodi/xbmc/xbmc/cores/DllLoader/exports/wrapper.o $(OBJS)
else
	$(CXX) $(CFLAGS) $(LDFLAGS) -shared -g -o $(LIB_SHARED) $(OBJS)
endif

CLEAN_FILES = \
	$(LIB_SHARED) \

DISTCLEAN_FILES= \
	Makefile \

include ../../../Makefile.include

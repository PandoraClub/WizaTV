ARCH=arm-osx

OBJS +=libexif.o ExifParse.o IptcParse.o JpegParse.o
CFLAGS +=-D_LINUX -D_DLL -fPIC -O2
CXXFLAGS +=-D_LINUX -D_DLL -fPIC -O2

SYSDIR=/Users/osx/Desktop/kodi/xbmc/system
SO=libexif-$(ARCH).so
SLIB=/Users/osx/Desktop/kodi/xbmc/system/$(SO)

$(SLIB): $(OBJS)
ifeq ($(findstring osx,$(ARCH)), osx)
	$(CXX) $(LDFLAGS) -Wl,-alias_list,/Users/osx/Desktop/kodi/xbmc/xbmc/cores/DllLoader/exports/wrapper_mach_alias \
		-bundle -undefined dynamic_lookup -read_only_relocs suppress -o $@ \
		/Users/osx/Desktop/kodi/xbmc/xbmc/cores/DllLoader/exports/wrapper.o $(OBJS)
else
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared -o $@ $(OBJS) -Wl,--unresolved-symbols=ignore-all \
		`cat /Users/osx/Desktop/kodi/xbmc/xbmc/cores/DllLoader/exports/wrapper.def` /Users/osx/Desktop/kodi/xbmc/xbmc/cores/DllLoader/exports/wrapper.o
endif

include /Users/osx/Desktop/kodi/xbmc/Makefile.include

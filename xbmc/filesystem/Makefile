CXXFLAGS += -D__STDC_FORMAT_MACROS

SRCS  = AddonsDirectory.cpp
SRCS += CacheStrategy.cpp
SRCS += CircularCache.cpp
SRCS += CDDADirectory.cpp
SRCS += CDDAFile.cpp
SRCS += CurlFile.cpp
SRCS += DAVCommon.cpp
SRCS += DAVDirectory.cpp
SRCS += DAVFile.cpp
SRCS += Directory.cpp
SRCS += DirectoryCache.cpp
SRCS += DirectoryFactory.cpp
SRCS += DirectoryHistory.cpp
SRCS += DllLibCurl.cpp
SRCS += EventsDirectory.cpp
SRCS += FavouritesDirectory.cpp
SRCS += File.cpp
SRCS += FileCache.cpp
SRCS += FileDirectoryFactory.cpp
SRCS += FileFactory.cpp
SRCS += FileReaderFile.cpp
SRCS += FTPDirectory.cpp
SRCS += FTPParse.cpp
SRCS += HTTPDirectory.cpp
SRCS += HTTPFile.cpp
SRCS += IDirectory.cpp
SRCS += IFile.cpp
SRCS += ImageFile.cpp
SRCS += iso9660.cpp
SRCS += ISO9660Directory.cpp
SRCS += ISOFile.cpp
SRCS += LibraryDirectory.cpp
SRCS += MemBufferCache.cpp
SRCS += MultiPathDirectory.cpp
SRCS += MultiPathFile.cpp
SRCS += MusicDatabaseDirectory.cpp
SRCS += MusicDatabaseFile.cpp
SRCS += MusicFileDirectory.cpp
SRCS += MusicSearchDirectory.cpp
SRCS += OverrideDirectory.cpp
SRCS += OverrideFile.cpp
SRCS += PlaylistDirectory.cpp
SRCS += PlaylistFileDirectory.cpp
SRCS += PipeFile.cpp
SRCS += PipesManager.cpp
SRCS += PluginDirectory.cpp
SRCS += posix/PosixDirectory.cpp
SRCS += posix/PosixFile.cpp
SRCS += PVRFile.cpp
SRCS += PVRDirectory.cpp
SRCS += ResourceDirectory.cpp
SRCS += ResourceFile.cpp
SRCS += RSSDirectory.cpp
SRCS += SAPDirectory.cpp
SRCS += SAPFile.cpp
SRCS += SFTPDirectory.cpp
SRCS += SFTPFile.cpp
SRCS += ShoutcastFile.cpp
SRCS += SmartPlaylistDirectory.cpp
SRCS += SourcesDirectory.cpp
SRCS += SpecialProtocol.cpp
SRCS += SpecialProtocolDirectory.cpp
SRCS += SpecialProtocolFile.cpp
SRCS += StackDirectory.cpp
SRCS += udf25.cpp
SRCS += UDFDirectory.cpp
SRCS += UDFFile.cpp
SRCS += VideoDatabaseDirectory.cpp
SRCS += VirtualDirectory.cpp
SRCS += XbtDirectory.cpp
SRCS += XbtFile.cpp
SRCS += XbtManager.cpp
SRCS += ZeroconfDirectory.cpp
SRCS += ZipDirectory.cpp
SRCS += ZipFile.cpp
SRCS += ZipManager.cpp

ifeq (,1)
SRCS += APKDirectory.cpp
SRCS += APKFile.cpp
SRCS += AndroidAppFile.cpp
SRCS += AndroidAppDirectory.cpp
endif

ifeq (1,1)
SRCS += RarFile.cpp
SRCS += RarDirectory.cpp
SRCS += RarManager.cpp
endif

ifeq (1,1)
SRCS += SMBFile.cpp
SRCS += SMBDirectory.cpp
endif

ifeq (1,1)
SRCS += NFSFile.cpp
SRCS += NFSDirectory.cpp
endif

ifeq (1,1)
SRCS += BlurayDirectory.cpp
SRCS += BlurayFile.cpp
endif

ifeq (1,1)
INCLUDES+=-I/Users/osx/Desktop/kodi/xbmc/lib/libUPnP
INCLUDES+=-I/Users/osx/Desktop/kodi/xbmc/lib/libUPnP/Platinum/Source/Core
INCLUDES+=-I/Users/osx/Desktop/kodi/xbmc/lib/libUPnP/Platinum/Source/Platinum
INCLUDES+=-I/Users/osx/Desktop/kodi/xbmc/lib/libUPnP/Platinum/Source/Devices/MediaConnect
INCLUDES+=-I/Users/osx/Desktop/kodi/xbmc/lib/libUPnP/Platinum/Source/Devices/MediaRenderer
INCLUDES+=-I/Users/osx/Desktop/kodi/xbmc/lib/libUPnP/Platinum/Source/Devices/MediaServer
INCLUDES+=-I/Users/osx/Desktop/kodi/xbmc/lib/libUPnP/Platinum/Source/Extras
INCLUDES+=-I/Users/osx/Desktop/kodi/xbmc/lib/libUPnP/Neptune/Source/System/Posix
INCLUDES+=-I/Users/osx/Desktop/kodi/xbmc/lib/libUPnP/Neptune/Source/Core
SRCS += NptXbmcFile.cpp
SRCS += UPnPDirectory.cpp
SRCS += UPnPFile.cpp
endif

LIB = filesystem.a

include /Users/osx/Desktop/kodi/xbmc/Makefile.include
-include $(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(SRCS)))

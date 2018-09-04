set(CMAKE_SYSTEM_VERSION 1)
set(OS "ios")
set(CPU "armv7")
set(PLATFORM "")
if("${OS}" STREQUAL "linux" OR "${OS}" STREQUAL "android")
  set(CMAKE_SYSTEM_NAME Linux)
endif()

set(CMAKE_FIND_ROOT_PATH @CMAKE_FIND_ROOT_PATH@)

# set special CORE_SYSTEM_NAME
if("${OS}" STREQUAL "android")
  set(CORE_SYSTEM_NAME android)
  list(APPEND CMAKE_FIND_ROOT_PATH /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/sysroot/usr)
elseif("${OS}" STREQUAL "osx")
  set(CORE_SYSTEM_NAME osx)
elseif("${OS}" STREQUAL "ios")
  set(CORE_SYSTEM_NAME ios)
elseif("${PLATFORM}" STREQUAL "raspberry-pi")
  set(CORE_SYSTEM_NAME rbpi)
  list(APPEND CMAKE_FIND_ROOT_PATH /opt/vc)
  set(CMAKE_LIBRARY_PATH @CMAKE_FIND_ROOT_PATH@/lib:/opt/vc/lib)
  set(CMAKE_INCLUDE_PATH @CMAKE_FIND_ROOT_PATH@/include:/opt/vc/include)
endif()


if("${OS}" STREQUAL "ios" OR "${OS}" STREQUAL "osx")
  set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS9.2.sdk)
  list(APPEND CMAKE_FIND_ROOT_PATH ${CMAKE_OSX_SYSROOT} ${CMAKE_OSX_SYSROOT}/usr /usr/X11R6)
  set(CMAKE_LIBRARY_PATH @CMAKE_FIND_ROOT_PATH@/lib:/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS9.2.sdk/lib:/usr/X11R6/lib)
  set(CMAKE_INCLUDE_PATH @CMAKE_FIND_ROOT_PATH@/include:/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS9.2.sdk/include:/usr/X11R6/include)
endif()

# specify the cross compiler
set(CMAKE_C_COMPILER   /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang)
set(CMAKE_CXX_COMPILER /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++)
set(CMAKE_AR /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ar CACHE FILEPATH "Archiver")
set(CMAKE_LINKER /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ld CACHE FILEPATH "Linker")
set(CMAKE_C_FLAGS "-fheinous-gnu-extensions -no-cpp-precomp -mcpu=cortex-a8 -mfpu=neon -ftree-vectorize -mfloat-abi=softfp -pipe -Wno-trigraphs -fpascal-strings -O3 -Wreturn-type -Wunused-variable -fmessage-length=0 -gdwarf-2 -arch armv7 -miphoneos-version-min=5.1  -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS9.2.sdk")
set(CMAKE_CXX_FLAGS "-no-cpp-precomp -mcpu=cortex-a8 -mfpu=neon -ftree-vectorize -mfloat-abi=softfp -pipe -Wno-trigraphs -fpascal-strings -O3 -Wreturn-type -Wunused-variable -fmessage-length=0 -gdwarf-2 -arch armv7 -miphoneos-version-min=5.1 -std=c++11 -stdlib=libc++ -g -O2 -std=gnu++11 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS9.2.sdk")
set(CMAKE_CPP_FLAGS "-fheinous-gnu-extensions -no-cpp-precomp -mcpu=cortex-a8 -mfpu=neon -ftree-vectorize -mfloat-abi=softfp -pipe -Wno-trigraphs -fpascal-strings -O3 -Wreturn-type -Wunused-variable -fmessage-length=0 -gdwarf-2 -arch armv7 -miphoneos-version-min=5.1  -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS9.2.sdk")
set(CMAKE_LD_FLAGS "-Wl,-search_paths_first -L/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS9.2.sdk/usr/lib/system -Wl,-segalign,4000 -arch armv7 -miphoneos-version-min=5.1 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS9.2.sdk  ")
set(ENV{CFLAGS} ${CMAKE_C_FLAGS})
set(ENV{CXXFLAGS} ${CMAKE_CXX_FLAGS})
set(ENV{CPPFLAGS} ${CMAKE_CPP_FLAGS})
set(ENV{LDFLAGS} ${CMAKE_LD_FLAGS})

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

if(NOT OS STREQUAL "linux")
  set(ADDONS_PREFER_STATIC_LIBS ON)
endif()

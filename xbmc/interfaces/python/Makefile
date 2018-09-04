ifeq (0,1)
DOXYGEN=none
endif

TOPDIR = /Users/osx/Desktop/kodi/xbmc
INTERFACES_DIR = /Users/osx/Desktop/kodi/xbmc/xbmc/interfaces
JAVA=/usr/bin/java
SWIG=/Users/Shared/xbmc-depends/buildtools-native/bin/swig
LIB=python_binding.a
all: $(LIB)

include ../../../codegenerator.mk

SRCS =	AddonPythonInvoker.cpp \
	CallbackHandler.cpp \
	ContextItemAddonInvoker.cpp \
	LanguageHook.cpp \
	PythonInvoker.cpp \
	XBPython.cpp \
	swig.cpp \
	PyContext.cpp \
	$(GENERATED)

INCLUDES += -I/Users/Shared/xbmc-depends/iphoneos9.2_armv7-target/include/python2.6
.SECONDARY: $(GENERATED)

include ../../../Makefile.include
-include $(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(SRCS)))


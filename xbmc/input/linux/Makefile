SRCS=LIRC.cpp \
  LinuxInputDevices.cpp

# xkbcommon detail
ifeq (,1)
SRCS += XKBCommonKeymap.cpp
endif

LIB=input_linux.a

include ../../../Makefile.include
-include $(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(SRCS)))

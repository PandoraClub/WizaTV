SRCS=	\
	TestSwig.cpp

LIB=pythonSwigTest.a

INCLUDES += -I../../../../lib/gtest/include
INCLUDES += -I/Users/Shared/xbmc-depends/iphoneos9.2_armv7-target/include/python2.6

include ../../../../Makefile.include
-include $(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(SRCS)))

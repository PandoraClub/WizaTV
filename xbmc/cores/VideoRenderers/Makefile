SRCS  = BaseRenderer.cpp
SRCS += OverlayRenderer.cpp
SRCS += OverlayRendererUtil.cpp
SRCS += OverlayRendererGUI.cpp
SRCS += RenderCapture.cpp
SRCS += RenderManager.cpp
SRCS += RenderFlags.cpp

ifeq ($(findstring arm,arm-osx),arm)
SRCS += yuv2rgb.neon.S
endif

ifeq (0,1)
SRCS += LinuxRendererGL.cpp
SRCS += OverlayRendererGL.cpp
endif

ifeq (1,1)
SRCS += LinuxRendererGLES.cpp
SRCS += OverlayRendererGL.cpp
endif

ifeq (,1)
SRCS += MMALRenderer.cpp
endif

LIB = VideoRenderer.a

include /Users/osx/Desktop/kodi/xbmc/Makefile.include
-include $(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(patsubst %.S,,$(SRCS)))) 

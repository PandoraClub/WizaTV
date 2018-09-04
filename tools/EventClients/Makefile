WII_EXTRA_OPTS=
CLEAN_FILES=Clients/WiiRemote/WiiRemote
PYTHON_SITEDIR := $(shell python -c "from distutils.sysconfig import get_python_lib; print get_python_lib()")

all: install

wiimote: Clients/WiiRemote/WiiRemote

Clients/WiiRemote/WiiRemote:
	cd Clients/WiiRemote && \
	$(CXX) $(CXXFLAGS) $(LDFLAGS) CWIID_WiiRemote.cpp -lcwiid -lbluetooth -DICON_PATH="\"$(datarootdir)/pixmaps/kodi/\"" $(WII_EXTRA_OPTS) -o WiiRemote

j2me-remote:
	cd Clients/J2ME\ Client
	ant -f build.xml

install: Clients/WiiRemote/WiiRemote
	mkdir -p $(DESTDIR)$(bindir)
	cp -a Clients/WiiRemote/WiiRemote $(DESTDIR)$(bindir)/kodi-wiiremote
	cp -a Clients/J2ME\ Client/j2me_remote.py $(DESTDIR)$(bindir)/kodi-j2meremote
	cp -a Clients/PS3\ BD\ Remote/ps3_remote.py $(DESTDIR)$(bindir)/kodi-ps3remote
	cp -a Clients/PS3\ Sixaxis\ Controller/ps3d.py $(DESTDIR)$(bindir)/kodi-ps3d
	cp -a Clients/Kodi\ Send/kodi-send.py $(DESTDIR)$(bindir)/kodi-send
	mkdir -p $(DESTDIR)$(PYTHON_SITEDIR)/kodi
	echo 'ICON_PATH="$(datarootdir)/pixmaps/kodi/"' > $(DESTDIR)$(PYTHON_SITEDIR)/kodi/defs.py
	cp -a lib/python/* $(DESTDIR)$(PYTHON_SITEDIR)/kodi/
	cp -a Clients/PS3\ BD\ Remote/ps3_remote.py $(DESTDIR)$(PYTHON_SITEDIR)/kodi/
	mkdir -p $(DESTDIR)$(includedir)/kodi
	cp -a lib/c++/* $(DESTDIR)$(includedir)/kodi/
	mkdir -p $(DESTDIR)$(datarootdir)/pixmaps/kodi
	cp -a icons/* $(DESTDIR)$(datarootdir)/pixmaps/kodi/

include ../../Makefile.include

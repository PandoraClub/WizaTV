#!/bin/sh

# usage: ./mkdmg-osx.sh release/debug (case insensitive)
# Allows us to run mkdmg-osx.sh from anywhere in the three, rather than the tools/darwin/packaging/osx folder only
SWITCH=`echo $1 | tr [A-Z] [a-z]`
DIRNAME=`dirname $0`

if [ ${SWITCH:-""} = "debug" ]; then
  echo "Packaging Debug target for OSX"
  APP="$DIRNAME/../../../../build/Debug/Kodi.app"
elif [ ${SWITCH:-""} = "release" ]; then
  echo "Packaging Release target for OSX"
  APP="$DIRNAME/../../../../build/Release/Kodi.app"
else
  echo "You need to specify the build target"
  exit 1 
fi

if [ ! -d $APP ]; then
  echo "Kodi.app not found! are you sure you built $1 target?"
  exit 1
fi
ARCHITECTURE=`file $APP/Contents/MacOS/Kodi | awk '{print $NF}'`

PACKAGE=org.xbmc.kodi-osx

VERSION=16.1
REVISION=0

if [ "" != "" ]; then
  REVISION=$REVISION~
fi

ARCHIVE=${PACKAGE}_${VERSION}-${REVISION}_macosx-intel-${ARCHITECTURE}

echo Creating $PACKAGE package version $VERSION revision $REVISION
${SUDO} rm -rf $DIRNAME/$ARCHIVE

if [ -e "/Volumes/$ARCHIVE" ]
then 
  umount /Volumes/$ARCHIVE
fi

#generate volume iconset
if [ `which iconutil` ]
then
  echo "Generating volumeIcon.icns"
  iconutil -c icns --output "VolumeIcon.icns" "../media/osx/volumeIcon.iconset"
fi

$DIRNAME/dmgmaker.pl $APP $ARCHIVE

echo "done"

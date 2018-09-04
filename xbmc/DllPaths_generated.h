#ifndef DLL_PATHS_GENERATED_H_
#define DLL_PATHS_GENERATED_H_

/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

/* prefix install location */
#define PREFIX_USR_PATH        "/Users/Shared/xbmc-depends/iphoneos9.2_armv7-target"

/* libraries */
#define DLL_PATH_CPLUFF        "special://xbmcbin/system/libcpluff-arm-osx.so"
#define DLL_PATH_IMAGELIB      "special://xbmcbin/system/ImageLib-arm-osx.so"
#define DLL_PATH_LIBEXIF       "special://xbmcbin/system/libexif-arm-osx.so"

#define DLL_PATH_LIBRTMP       "librtmp.1.dylib"
#define DLL_PATH_LIBNFS        "libnfs.4.dylib"
#define DLL_PATH_LIBGIF        "libgif.7.dylib"

#define DLL_PATH_LIBPLIST      "libplist.1.dylib"
#define DLL_PATH_LIBSHAIRPLAY  "libshairplay.0.dylib"
#define DLL_PATH_LIBCEC        ""

#ifndef DLL_PATH_LIBCURL
#define DLL_PATH_LIBCURL       "libcurl.4.dylib"
#endif

/* dvdplayer */
#define DLL_PATH_LIBASS        "libass.5.dylib"
#define DLL_PATH_LIBDVDNAV     "special://xbmcbin/system/players/dvdplayer/libdvdnav-arm-osx.so"
#define DLL_PATH_LIBMPEG2      "libmpeg2.0.dylib"

/* libbluray */
#define DLL_PATH_LIBBLURAY     "libbluray.1.dylib"

/* wayland */
#define DLL_PATH_WAYLAND_CLIENT ""
#define DLL_PATH_WAYLAND_EGL ""

/* xkbcommon */
#define DLL_PATH_XKBCOMMON ""

/* sse4 */
#define DLL_PATH_LIBSSE4      "special://xbmcbin/system/libsse4-arm-osx.so"

#endif

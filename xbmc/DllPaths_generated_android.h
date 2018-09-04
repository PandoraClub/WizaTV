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
#pragma once
/* libraries */

//Android rules:
// - All solibs must be in the form ^lib*.so$
// - All solibs must be in the same dir
// - Arch need not be specified, each arch will get its own lib dir.
//   We only keep arm-osx here to retain the same structure as *nix.
// * foo.so will be renamed libfoo.so in the packaging stage

#define DLL_PATH_CPLUFF        "libcpluff-arm-osx.so"
#define DLL_PATH_IMAGELIB      "libImageLib-arm-osx.so"
#define DLL_PATH_LIBEXIF       "libexif-arm-osx.so"

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
#define DLL_PATH_LIBDVDNAV     "libdvdnav-arm-osx.so"
#define DLL_PATH_LIBMPEG2      "libmpeg2.0.dylib"
#define DLL_PATH_LIBSTAGEFRIGHTICS    "libXBMCvcodec_stagefrightICS-arm-osx.so"

/* libbluray */
#define DLL_PATH_LIBBLURAY     "libbluray.1.dylib"

/* Android's libui for gralloc */
#define DLL_PATH_LIBUI         "libui.so"

if(NOT EXISTS "/Users/osx/Desktop/kodi/xbmc/tools/depends/native/cmake-native/x86_64-osx-native/install_manifest.txt")
  message(FATAL_ERROR "Cannot find install manifest: \"/Users/osx/Desktop/kodi/xbmc/tools/depends/native/cmake-native/x86_64-osx-native/install_manifest.txt\"")
endif()

file(READ "/Users/osx/Desktop/kodi/xbmc/tools/depends/native/cmake-native/x86_64-osx-native/install_manifest.txt" files)
string(REPLACE "\n" ";" files "${files}")
foreach(file ${files})
  message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  if(EXISTS "$ENV{DESTDIR}${file}")
    exec_program(
      "/Users/osx/Desktop/kodi/xbmc/tools/depends/native/cmake-native/x86_64-osx-native/Bootstrap.cmk/cmake" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
      OUTPUT_VARIABLE rm_out
      RETURN_VALUE rm_retval
      )
    if("${rm_retval}" STREQUAL 0)
    else()
      message(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    endif()
  else()
    message(STATUS "File \"$ENV{DESTDIR}${file}\" does not exist.")
  endif()
endforeach()

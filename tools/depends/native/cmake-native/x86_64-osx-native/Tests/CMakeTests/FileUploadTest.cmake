file(REMOVE_RECURSE "/Users/osx/Desktop/kodi/xbmc/tools/depends/native/cmake-native/x86_64-osx-native/Tests/CMakeTests/uploads")

if(EXISTS "/Users/osx/Desktop/kodi/xbmc/tools/depends/native/cmake-native/x86_64-osx-native/Tests/CMakeTests/uploads/file1.png")
  message(FATAL_ERROR "error: file1.png exists - should have been deleted")
endif()
if(EXISTS "/Users/osx/Desktop/kodi/xbmc/tools/depends/native/cmake-native/x86_64-osx-native/Tests/CMakeTests/uploads/file2.png")
  message(FATAL_ERROR "error: file2.png exists - should have been deleted")
endif()

file(MAKE_DIRECTORY "/Users/osx/Desktop/kodi/xbmc/tools/depends/native/cmake-native/x86_64-osx-native/Tests/CMakeTests/uploads")

set(filename "/Users/osx/Desktop/kodi/xbmc/tools/depends/native/cmake-native/x86_64-osx-native/Tests/CMakeTests/FileDownloadInput.png")
set(urlbase "file:///Users/osx/Desktop/kodi/xbmc/tools/depends/native/cmake-native/x86_64-osx-native/Tests/CMakeTests/uploads")

message(STATUS "FileUpload:1")
file(UPLOAD
  ${filename}
  ${urlbase}/file1.png
  TIMEOUT 2
  )

message(STATUS "FileUpload:2")
file(UPLOAD
  ${filename}
  ${urlbase}/file2.png
  STATUS status
  LOG log
  SHOW_PROGRESS
  )

execute_process(COMMAND ${CMAKE_COMMAND} -E md5sum
  "/Users/osx/Desktop/kodi/xbmc/tools/depends/native/cmake-native/x86_64-osx-native/Tests/CMakeTests/uploads/file1.png"
  OUTPUT_VARIABLE sum1
  OUTPUT_STRIP_TRAILING_WHITESPACE)
if(NOT sum1 MATCHES "^d16778650db435bda3a8c3435c3ff5d1  .*/uploads/file1.png$")
  message(FATAL_ERROR "file1.png did not upload correctly (sum1='${sum1}')")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} -E md5sum
  "/Users/osx/Desktop/kodi/xbmc/tools/depends/native/cmake-native/x86_64-osx-native/Tests/CMakeTests/uploads/file2.png"
  OUTPUT_VARIABLE sum2
  OUTPUT_STRIP_TRAILING_WHITESPACE)
if(NOT sum2 MATCHES "^d16778650db435bda3a8c3435c3ff5d1  .*/uploads/file2.png$")
  message(FATAL_ERROR "file2.png did not upload correctly (sum2='${sum2}')")
endif()

message(STATUS "log='${log}'")
message(STATUS "status='${status}'")
message(STATUS "DONE")

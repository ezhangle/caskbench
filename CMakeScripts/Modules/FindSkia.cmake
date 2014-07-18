# Find the Skia library
#
# This defines:
#
#    SKIA_FOUND
#    SKIA_INCLUDE_DIRS
#    SKIA_LIBRARIES

message("Looking for skia...")
set(SKIA_BUILD_DIR "" CACHE PATH "Skia build directory")

find_library(SKIA_LIBRARIES
  NAMES skia
  PATHS
    ${SKIA_BUILD_DIR}/out/Release
    ${SKIA_BUILD_DIR}/trunk/out/Release
    $ENV{SKIA_DIR}/lib
    $ENV{SKIA_DIR}
    /usr/local/lib
    /usr/lib
    /opt/lib
  )
find_path(SKIA_INCLUDE_DIR
  NAMES core/SkCanvas.h
  PATH_SUFFIXES skia
  PATHS
    ${SKIA_BUILD_DIR}/include
    ${SKIA_BUILD_DIR}/trunk/include
    $ENV{SKIA_DIR}/include
    $ENV{SKIA_DIR}
    /usr/local/include
    /usr/include
    /opt/include
  )

if (SKIA_INCLUDE_DIR)
  message("Found include base ${SKIA_INCLUDE_DIR}")
endif (SKIA_INCLUDE_DIR)


set(SKIA_INCLUDE_DIRS "")
list(APPEND SKIA_INCLUDE_DIRS ${SKIA_INCLUDE_DIR}/)
list(APPEND SKIA_INCLUDE_DIRS ${SKIA_INCLUDE_DIR}/core/)
list(APPEND SKIA_INCLUDE_DIRS ${SKIA_INCLUDE_DIR}/config/)
list(APPEND SKIA_INCLUDE_DIRS ${SKIA_INCLUDE_DIR}/effects/)
list(APPEND SKIA_INCLUDE_DIRS ${SKIA_INCLUDE_DIR}/utils/)
list(APPEND SKIA_INCLUDE_DIRS ${SKIA_INCLUDE_DIR}/tools/flags/)
list(APPEND SKIA_INCLUDE_DIRS ${SKIA_INCLUDE_DIR}/lazy/)
list(APPEND SKIA_INCLUDE_DIRS ${SKIA_INCLUDE_DIR}/gpu/)
list(APPEND SKIA_INCLUDE_DIRS ${SKIA_INCLUDE_DIR}/gpu/gl/)
list(APPEND SKIA_INCLUDE_DIRS ${SKIA_INCLUDE_DIR}/../src/effects/)

if (SKIA_LIBRARIES)
  message("Found libraries")
endif (SKIA_LIBRARIES)

if (SKIA_INCLUDE_DIRS)
  message("Found includes")
endif (SKIA_INCLUDE_DIRS)

set (SKIA_FOUND "NO")
if (SKIA_LIBRARIES AND SKIA_INCLUDE_DIRS)
  set (SKIA_FOUND "YES")
else (SKIA_LIBRARIES AND SKIA_INCLUDE_DIRS)
  if (Skia_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find Skia")
  endif (Skia_FIND_REQUIRED)
endif (SKIA_LIBRARIES AND SKIA_INCLUDE_DIRS)

message("Done looking at skia")

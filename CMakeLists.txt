cmake_minimum_required(VERSION 2.8)

project(caskbench)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(PkgConfig REQUIRED)

#add_subdirectory(src)

pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
find_package(X11 REQUIRED)

include_directories(${GTK2_INCLUDE_DIRS})
link_directories(${GTK2_LIBRARY_DIRS})
add_definitions(${GTK2_CFLAGS_OTHER})

#message("CMake Prefix: ${CMAKE_PREFIX_PATH}")

find_package(Cairo)
#include_directories(BEFORE ${CAIRO_INCLUDE_DIRS})

add_executable(fill src/fill.cpp)
target_link_libraries(fill cairo)

add_executable(image src/image.cpp)
target_link_libraries(image cairo)

add_executable(mask src/mask.cpp)
target_link_libraries(mask cairo)

add_executable(paint src/paint.cpp)
target_link_libraries(paint cairo)

add_executable(stroke src/stroke.cpp)
target_link_libraries(stroke ${CAIRO_LIBRARIES} GL ${GTK2_LIBRARIES} ${X11_LIBRARIES})

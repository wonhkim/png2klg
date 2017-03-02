cmake_minimum_required(VERSION 2.8)

set(project_name "PNG2KLG")
set(exe_name "png2klg")

find_package(ZLIB REQUIRED)
find_package(OpenCV REQUIRED)

IF(UNIX)
    set(CMAKE_CXX_FLAGS "-O3 -msse2 -msse3")
ENDIF (UNIX)

INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "-O3 -msse2 -msse3 -std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath,. -L${CMAKE_CURRENT_BINARY_DIR}")

set(srcs main.cpp LogWriter.cpp ImageUtil.cpp)

add_executable(${exe_name} ${srcs})

target_link_libraries(${exe_name}
                      ${ZLIB_LIBRARY}
                      ${OpenCV_LIBS} 
                      jpeg
                      )


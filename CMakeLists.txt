cmake_minimum_required(VERSION 3.10)
#project name
PROJECT(dc)
#head file
INCLUDE_DIRECTORIES(/usr/local/src/gmp-6.2.1)
INCLUDE_DIRECTORIES(include)
#source directory
#SET(SOURCE main.cpp GMPecdsa.cpp GMPell.cpp)
file(GLOB SOURCE "src/*.cpp" )
SET(EXECUTABLE_OUTPUT_PATH /home/keln/project/dc/bin)
SET(LIBRARY_OUTPUT_PATH /home/keln/project/dc/lib)
#SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_BUILD_TYPE "Release")

SET(PROJECT_LINK_LIBS libssl.so libgmp.so libcrypto.so)

LINK_DIRECTORIES(/usr/local/lib)

ADD_EXECUTABLE(ecdsa ${SOURCE})

target_link_libraries(ecdsa ${PROJECT_LINK_LIBS})


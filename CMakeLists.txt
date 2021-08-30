cmake_minimum_required(VERSION 3.17)
project(adapi C)

set(CMAKE_C_STANDARD 11)

add_executable(adapi main.c logger.c logger.h socket.c socket.h main.h cmake-build-debug/utils.c cmake-build-debug/utils.h)

target_link_libraries(adapi ssl crypto)
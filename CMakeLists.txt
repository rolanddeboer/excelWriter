cmake_minimum_required(VERSION 3.10)
project(excelWriter VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_COMPILER musl-gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")

include_directories(include)
find_package(ZLIB REQUIRED)

add_executable(excelWriter main.cpp)

target_link_libraries(excelWriter ${PROJECT_SOURCE_DIR}/lib/libxlsxwriter.a /usr/lib/x86_64-linux-gnu/libz.a)

install(TARGETS excelWriter DESTINATION ~/bin)


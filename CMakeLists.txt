cmake_minimum_required(VERSION 3.10)
project(excelWriter VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)
find_package(ZLIB REQUIRED)
add_executable(excelWriter main.cpp)

target_link_libraries(excelWriter ${PROJECT_SOURCE_DIR}/lib/libxlsxwriter.a ZLIB::ZLIB)

install(TARGETS excelWriter DESTINATION ~/bin)
cmake_minimum_required(VERSION 3.12)  # Minimum version of CMake required

project(EchoSymbolService)
find_package(Threads REQUIRED)
add_executable(EchoSymbolService main.cpp)
target_link_libraries(EchoSymbolService PRIVATE Threads::Threads)

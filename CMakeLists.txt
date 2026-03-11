cmake_minimum_required(VERSION 3.10)
project(linapp-ws)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED libadwaita-1)

add_executable(linapp-ws src/main.cpp)

target_include_directories(linapp-ws PRIVATE ${GTK4_INCLUDE_DIRS})
target_link_libraries(linapp-ws ${GTK4_LIBRARIES})

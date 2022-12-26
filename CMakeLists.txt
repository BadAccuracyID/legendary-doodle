cmake_minimum_required(VERSION 3.23)
project(gniniarTerPSD)

set(CMAKE_CXX_STANDARD 11)

add_executable(Test1 Tests/test1.cpp)
add_executable(Test2 Tests/test2.cpp)

add_executable(H1 H1/main.cpp)
add_executable(H2 H2/main.cpp)
add_executable(H3 H3/main.cpp)
add_executable(H4 H4/main.cpp)

add_executable(Project Project/main.cpp Project/utils/Utils.cpp Project/utils/Utils.h Project/database/Database.cpp Project/database/Database.h Project/manager/FileManager.cpp Project/manager/FileManager.h)

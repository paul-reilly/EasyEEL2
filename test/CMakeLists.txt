cmake_minimum_required(VERSION 3.10)

project(easyeel2-test)

add_executable(test
  EEL_Test.cpp
)

add_subdirectory(../ eel-build)


target_link_libraries(test easyeel2)
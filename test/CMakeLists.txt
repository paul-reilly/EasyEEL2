cmake_minimum_required(VERSION 3.00)

project(easyeel2-test)

add_executable(test
  EEL_Test.cpp
)

add_subdirectory(../ eel-build)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(SRCS
    ../WDL/swell/swell.h
    ../WDL/swell/swell.cpp
  )
else()
  set( LINK_FLAGS 
      "/MACHINE:X64 /Zc:inline /fp:precise" 
  )
endif()

target_link_libraries(test easyeel2)

set(CMAKE_CXX_FLAGS "-DWDL_NO_DEFINE_MINMAX")
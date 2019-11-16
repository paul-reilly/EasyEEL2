cmake_minimum_required(VERSION 3.13)

project(easyeel2-test)

add_executable(test
  EEL_Test.cpp
)

set(CMAKE_CXX_FLAGS "-DWDL_NO_DEFINE_MINMAX")

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_compile_options(test PRIVATE
    -no-rtti
  )
else()
  target_compile_options(test PRIVATE
    /EHsc /GR-
  )
  target_link_options(test PRIVATE
    /machine:x64
  )
endif()

add_subdirectory(../ eel2lib-build)
target_link_libraries(test easyeel2)


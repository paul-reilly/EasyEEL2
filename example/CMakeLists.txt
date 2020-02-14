cmake_minimum_required(VERSION 3.13)

project(easyeel2-test)

add_executable(example
  EEL_Test.cpp
)

include_directories(../external)

target_compile_definitions(example PRIVATE
  -DWDL_NO_DEFINE_MINMAX -DDOCTEST_CONFIG_DISABLE -D_CRT_SECURE_NO_WARNINGS
)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_compile_options(example PRIVATE
    -fno-rtti
  )
else()
  target_compile_options(example PRIVATE
    /EHsc /GR-
  )
  target_link_options(example PRIVATE
    /machine:x64
  )
endif()

add_subdirectory(../ eel2lib-build EXCLUDE_FROM_ALL)
target_link_libraries(example easyeel2)

add_custom_command(TARGET example POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink
  ${CMAKE_CURRENT_SOURCE_DIR}/script.eel
  ${CMAKE_CURRENT_BINARY_DIR}/script.eel
  COMMENT "Creating symlink for script.eel at: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)


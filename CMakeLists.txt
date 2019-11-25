cmake_minimum_required(VERSION 3.13)

project(EasyEEL2)

enable_testing()

set(SRCS
  EasyEEL.cpp
  test-script.eel
)

include_directories(deps)

add_library(common_files OBJECT
  deps/WDL/lineparse.h
  deps/WDL/assocarray.h
  deps/WDL/chunkalloc.h
  deps/WDL/fft.c
  deps/WDL/eel2/nseel-caltab.c
  deps/WDL/eel2/nseel-cfunc.c
  deps/WDL/eel2/nseel-compiler.c
  deps/WDL/eel2/nseel-eval.c
  deps/WDL/eel2/nseel-lextab.c
  deps/WDL/eel2/nseel-ram.c
  deps/WDL/eel2/nseel-yylex.c
  #deps/WDL/eel2/ns-eel.h
)

add_library(easyeel2 STATIC ${SRCS} $<TARGET_OBJECTS:common_files> )
set_target_properties(easyeel2 PROPERTIES CXX_STANDARD 17)
add_executable(tests ${SRCS})
set_target_properties(tests PROPERTIES CXX_STANDARD 17 EXCLUDE_FROM_ALL True)
target_compile_definitions(easyeel2 PRIVATE -DDOCTEST_CONFIG_DISABLE)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_sources(easyeel2 PRIVATE deps/WDL/eel2/asm-nseel-x64.o)
  set(OPTS -Wno-format-truncation)
  target_compile_options(common_files PRIVATE ${OPTS})
  target_compile_options(easyeel2 PRIVATE ${OPTS})
  target_compile_options(tests PRIVATE ${OPTS})
else()
  target_sources(easyeel2 PRIVATE deps/WDL/eel2/asm-nseel-x64.obj)
  set(DEFS -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
  target_compile_definitions(common_files PRIVATE ${DEFS})
  target_compile_definitions(easyeel2 PRIVATE ${DEFS})
  target_compile_definitions(tests PRIVATE ${DEFS})
  target_link_options(easyeel2 PRIVATE /machine:x64)
endif()

target_link_libraries(tests PRIVATE easyeel2)

set(CMAKE_CXX_FLAGS 
  -DWDL_NO_DEFINE_MINMAX
)
add_test(NAME test COMMAND tests)

add_custom_target(easyeel2-libonly
    COMMAND ninja easyeel2
)

add_custom_command(TARGET tests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink
  ${CMAKE_CURRENT_SOURCE_DIR}/test-script.eel
  ${CMAKE_CURRENT_BINARY_DIR}/test-script.eel
  COMMENT "Creating symlink to test-script.eel at: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

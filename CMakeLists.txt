cmake_minimum_required(VERSION 3.13)

project(EasyEEL2)

set(SRCS
  src/EasyEEL.cpp
  include/EasyEEL.h
  tests/test-script.eel
)

add_library(common_files OBJECT
  external/WDL/lineparse.h
  external/WDL/assocarray.h
  external/WDL/chunkalloc.h
  external/WDL/fft.c
  external/WDL/eel2/nseel-caltab.c
  external/WDL/eel2/nseel-cfunc.c
  external/WDL/eel2/nseel-compiler.c
  external/WDL/eel2/nseel-eval.c
  external/WDL/eel2/nseel-lextab.c
  external/WDL/eel2/nseel-ram.c
  external/WDL/eel2/nseel-yylex.c
  #external/WDL/eel2/ns-eel.h
)
target_include_directories(common_files PRIVATE external)

add_library(easyeel2 STATIC ${SRCS} $<TARGET_OBJECTS:common_files> )
target_compile_features(easyeel2 PRIVATE cxx_std_17)
target_compile_definitions(easyeel2 PRIVATE -DDOCTEST_CONFIG_DISABLE)
target_include_directories(easyeel2 PRIVATE include external)

add_executable(tests ${SRCS})
target_compile_features(tests PRIVATE cxx_std_17)
set_target_properties(tests PROPERTIES EXCLUDE_FROM_ALL True)
target_include_directories(tests PRIVATE include external)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_sources(easyeel2 PRIVATE external/WDL/eel2/asm-nseel-x64.o)
  set(OPTS -Wno-format-truncation)
  target_compile_options(common_files PRIVATE ${OPTS})
  target_compile_options(easyeel2 PRIVATE ${OPTS})
  target_compile_options(tests PRIVATE ${OPTS})
else()
  target_sources(easyeel2 PRIVATE external/WDL/eel2/asm-nseel-x64.obj)
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

add_custom_target(easyeel2-libonly
    COMMAND ninja easyeel2
)

if (CMAKE_GENERATOR MATCHES "Visual Studio")
  set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)
else()
  set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
endif()

add_custom_command(TARGET tests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test-script.eel
    ${OUTPUT_DIR}/test-script.eel
  COMMENT "Creating symlink to test-script.eel at: ${OUTPUT_DIR}"
)

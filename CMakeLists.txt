cmake_minimum_required(VERSION 3.13)

project(EasyEEL2)

add_library(easyeel2 STATIC
  EasyEEL.cpp
  WDL/assocarray.h
  WDL/chunkalloc.h
  WDL/fft.c
  WDL/eel2/nseel-caltab.c
  WDL/eel2/nseel-cfunc.c
  WDL/eel2/nseel-compiler.c
  WDL/eel2/nseel-eval.c
  WDL/eel2/nseel-lextab.c
  WDL/eel2/nseel-ram.c
  WDL/eel2/nseel-yylex.c
  WDL/eel2/ns-eel.h
)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_sources(easyeel2 PRIVATE
    WDL/swell/swell.h
    WDL/swell/swell.cpp
    WDL/filebrowse.cpp
    WDL/eel2/asm-nseel-x64.o
  )
  target_compile_options(easyeel2 PRIVATE
    -Wno-format-truncation
  )
else()
  target_sources(easyeel2 PRIVATE
    WDL/eel2/asm-nseel-x64.obj
    WDL/win32_utf8.c
    WDL/win7filedialog.cpp
  )
  target_compile_definitions(easyeel2 PRIVATE
    -D_CRT_SECURE_NO_WARNINGS
    -D_CRT_NONSTDC_NO_DEPRECATE
  )
  target_link_options(easyeel2 PRIVATE
      /machine:x64
  )
endif()

set(CMAKE_CXX_FLAGS 
  "-DWDL_NO_DEFINE_MINMAX"
)

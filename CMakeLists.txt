cmake_minimum_required(VERSION 3.00)

project(EasyEEL2)

set(SRCS
  EasyEel.cpp
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
  WDL/eel2/y.tab.c
  WDL/eel2/ns-eel.h
)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(SRCS
    ${SRCS}
    WDL/swell/swell.h
    WDL/swell/swell.cpp
    WDL/filebrowse.cpp
    WDL/eel2/asm-nseel-x64.o
  )
else()
  set(SRCS
    ${SRCS}
    WDL/eel2/asm-nseel-x64.obj
    WDL/win32_utf8.c
    WDL/win7filedialog.cpp
  )
  set( LINK_FLAGS 
      "/MACHINE:X64 /Zc:inline /fp:precise" 
  )
endif()

add_library(easyeel2 STATIC
  ${SRCS}
)

set(CMAKE_CXX_FLAGS 
  "-DWDL_NO_DEFINE_MINMAX"
)

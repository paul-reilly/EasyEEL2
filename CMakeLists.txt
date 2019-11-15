cmake_minimum_required(VERSION 3.00)

project(EasyEEL2)

set(SRCS
  EasyEel.cpp
  ./WDL/assocarray.h
  ./WDL/chunkalloc.h
  ./WDL/filebrowse.cpp
  ./WDL/fft.c
  ./WDL/win32_utf8.c
  ./WDL/win7filedialog.cpp
  ./WDL/eel2/asm-nseel-x64.obj
  ./WDL/eel2/eel_disasm.cpp
  ./WDL/eel2/loose_eel.cpp
  ./WDL/eel2/nseel-caltab.c
  ./WDL/eel2/nseel-cfunc.c
  ./WDL/eel2/nseel-compiler.c
  ./WDL/eel2/nseel-eval.c
  ./WDL/eel2/nseel-lextab.c
  ./WDL/eel2/nseel-ram.c
  ./WDL/eel2/nseel-yylex.c
  ./WDL/eel2/y.tab.c
)

add_library(easyeel2 STATIC
  ${SRCS}
)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set( CMAKE_FIND_LIBRARY_SUFFIXES .lib .dll )
    
	 target_link_libraries( easyeel2 PUBLIC
        winmm dsound kernel32 shlwapi comctl32 wininet shell32 user32 gdi32 winspool comdlg32 
          advapi32 ole32 oleaut32 uuid odbc32 odbccp32 
    )

    target_compile_definitions( easyeel2 PUBLIC
	    -DNOMINMAX -D_WIN32 -D_CRT_SECURE_NO_WARNINGS
        -DWIN32 -DAPP_API -D__WINDOWS_DS__ -D__WINDOWS_MM__ -D__WINDOWS_ASIO__ -D_MBCS 
        -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE 
    )
	set_property( TARGET easyeel2 APPEND_STRING PROPERTY LINK_FLAGS 
      "/MANIFEST /NXCOMPAT /DYNAMICBASE /SUBSYSTEM:WINDOWS /NOLOGO /MACHINE:X64 /Zc:inline /fp:precise" 
    )

endif()
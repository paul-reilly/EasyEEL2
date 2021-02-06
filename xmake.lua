set_project("EasyEEL2")
set_languages("c++17")

add_requires("doctest")

target("eel2")
   set_kind("object")
   add_files(
       "external/WDL/fft.c",
       "external/WDL/eel2/nseel-caltab.c",
       "external/WDL/eel2/nseel-cfunc.c",
       "external/WDL/eel2/nseel-compiler.c",
       "external/WDL/eel2/nseel-eval.c",
       "external/WDL/eel2/nseel-lextab.c",
       "external/WDL/eel2/nseel-ram.c",
       "external/WDL/eel2/nseel-yylex.c"
    )
    add_headerfiles(
       "external/WDL/lineparse.h",
       "external/WDL/assocarray.h",
       "external/WDL/chunkalloc.h"
    )
    add_includedirs("external/WDL", { public = true })
    add_defines("WDL_NO_DEFINE_MINMAX", { public = true })
    if is_plat("linux") then
        add_files("external/WDL/eel2/asm-nseel-x64.o")
    else
        add_files("external/WDL/eel2/asm-nseel-x64.obj")
        add_defines("_CRT_SECURE_NO_WARNINGS", "_CRT_NONSTDC_NO_DEPRECATE")
    end


target("easyeel2")
    set_kind("static")
    add_deps("eel2")
    add_packages("doctest")
    add_files("src/EasyEEL.cpp")
    add_defines("DOCTEST_CONFIG_DISABLE")
    add_includedirs("include/")


target("tests")
    set_default(false)
    set_kind("binary")
    add_deps("eel2")
    add_packages("doctest")
    set_rundir("tests")
    add_files("src/EasyEEL.cpp")
    add_includedirs("include/")


EEL2 is the JIT compiled scripting language and implementation from [Cockos Inc's WDL framework](https://www.cockos.com/wdl/).

This EasyEEL2 library is for convenient inclusion and usage and doesn't intend to offer the flexibility of the underlying library. Include as a git submodule and use as a CMake subdirectory:

```git
git submodule add https://github.com/paul-reilly/EasyEEL2.git
```
*CMakeLists.txt:*
```cmake
add_subdirectory(./EasyEEL2 [choose a build subdir])
target_link_libraries(main easyeel2)
target_include_directories(main ./EasyEEL2/include)
```


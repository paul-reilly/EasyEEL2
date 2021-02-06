[![Build status](https://ci.appveyor.com/api/projects/status/o7fom4c0rswr1cs7/branch/master?svg=true)](https://ci.appveyor.com/project/paul-reilly/easyeel2/branch/master)

EEL2 is the JIT compiled scripting language and implementation from [Cockos Inc's WDL framework](https://www.cockos.com/wdl/).

This EasyEEL2 library is for convenient inclusion and usage and doesn't intend to offer the flexibility of the underlying library.

**To build library:**
```
xmake
```

**To build and run tests:**
```
xmake b[uild] tests
xmake r[un] tests
```

## Usage

```git
git submodule add https://github.com/paul-reilly/EasyEEL2.git
```
*xmake.lua*
```lua
includes("submodule_directory")

target("myapp")
  ...
  add_deps("EasyEel2")
  ...
```

**Note:**

If you need to use CMake, we can generate a CMakeLists.txt file with:
```lua
xmake project -k cmake
```


# The best game ever

[![Windows status](https://ci.appveyor.com/api/projects/status/h2wfkb1y546x5tsw/branch/master?svg=true)](https://ci.appveyor.com/project/onqtam/game/branch/master)
[![Linux Status](https://travis-ci.org/onqtam/game.svg?branch=master)](https://travis-ci.org/onqtam/game)
[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/)
[![License](http://img.shields.io/badge/license-MIT-blue.svg)](http://opensource.org/licenses/MIT)

This is a repository of a future game and game engine. It will use only open technologies and will be multiplatform.
Sometime in the distant future (1-2 years) shall go private when progress starts to speed up and a demo takes shape.

## Building

You'll need:

- **Python 2.7/3.x**
    - you'll also need the ```colorama``` package - ```pip install colorama```
- **CMake 3.0** or newer
- Compilers (one of them)
    - **VS 2017**
    - **g++ 7**
    - **clang 5** (call ```export CXX=clang++``` before calling ```ha -g gcc```)
- Required packages for ubuntu: ```xorg-dev```, ```libx11-dev```, ```libgl1-mesa-dev```

Use the ```ha``` python script in the root of the repository to build the project (it has help). Emscripten requires **Python 2.7** and has been setup to work only on windows.

```
ha -s        # setup repository
ha -g msvc   # generate msvc solution
ha -b gcc    # build with gcc or clang (implicitly calls "ha -g gcc" to generate makefiles)
```

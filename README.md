# C++ Ballistic Calculator

This program implements the external ballistic 
model described 
in [this paper](https://github.com/mazonka/artill/raw/master/drag_anziam.pdf).

The program is written in standard C++11 with 
one library dependency 
on [Dlib](http://dlib.net/) that is included in sources
(no need to download/install it separately).

## What can you do with this program

1. Define basic parameters of the projectile: mass, 
diameter, length; and muzzle velocity.
2. Define drag coefficient function. If the function 
is not known, there is an option to generate this 
function from any empirical data. See section Solve.
3. Run ballistic trajectories with different options: 
wind, air turbulences, initial height above the Earth’s surface.
4. Solve for ranges or max – the programs finds 
the angles (for both flat and high trajectories) for a given range.
5. Tune the drag coefficient function given some firing table data.

 
## How to build 

### Automatic build

First, check out `src` directory. Build `dlad` and build `drag`. If you have `make`, type `make` and it should build. As a direct option use `make PLAT=w` for Windows or `make PLAT=u` for Unix builds. If you do not have `make`, then build manually.

### Unix manual build 

1. Unpack `dlib.tar.bz2`
2. Place `dlib` directory inside `dlad`.
3. Compile dlibad.cpp, e.g.` g++ -std=c++14 -c dlibad.cpp`
4. Compile all cpp files (except alglib.cpp, timer_inc.cpp, timer_w.cpp) 
inside `drag`, e.g. ` g++ -std=c++14 -c -I ../dlad *.cpp`. This example 
command will actually spits errors on those files not to be compiled – that is fine.
5. Copy ` dlibad.o` from `dlad` to `drag`
6. Link all `.o` files, e.g. ` g++ *.o -o drag`

### Windows manual build

If you have gcc installed, then follow Unix build instructions. If you use MS C++ compiler, check its version – it should be 19 or above:
```
Microsoft (R) C/C++ Optimizing Compiler Version 19.10.24821 for x86
Copyright (C) Microsoft Corporation.  All rights reserved.
```

1. Unpack `dlib.tar.bz2`
2. Place `dlib` directory inside `dlad`.
3. Compile dlibad.cpp, e.g.` cl -EHsc -c dlibad.cpp`
4. Compile all cpp files (except alglib.cpp, timer_inc.cpp, 
timer_u.cpp) inside `drag`, e.g. ` cl -EHsc -c -I ../dlad *.cpp`. 
This example command will actually spits errors on those files not 
to be compiled – that is fine.
5. Copy ` dlibad.obj` from `dlad` to `drag`
6. Link all `.obj` files, e.g. ` cl *.obj /Fedrag`


## How to run

TBD

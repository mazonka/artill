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

TBD

## How to run

TBD

# Zaxxon
Recreation of the popular [Zaxxon](https://wikipedia.org/wiki/Zaxxon) Sega arcade game in C++ \
An online playable version of the original Zaxxon rom can be found on [archive](https://archive.org/details/arcade_zaxxon).

## How to play
**Controls** \
(PC) - Arrow keys to control player, pitch is inverted. Z to shoot.\
(Arcade) - Stick to control player, pitch is inverted. Any button to shoot.

**Goal** \
Hit as many targets as you can without being hit or running out of fuel, each lap the games gets faster. \
Fuel is regained by destroying the red fuel tanks. \
There are 3 stages, the basic one with just obstacles, space with enemy planes to fight, and the final has a boss. \
High scores are saved and listed, see how high you can get!


## Build instructions
For Visual Studio 2022 (ISO C++ 14 standard): \
Clone the repository, set your build to x86 (32-bit), and build! \
To switch out [SFML](https://www.sfml-dev.org/) version, simply replace `lib/SFML` with your new version. No guranteed support for versions aside from SFML 2.6.1 32-bit version. \
The resulting exe file has all resources statically linked, so you can run the exe without any extra files.
Requires OpenAL32 dynamic library to be in the folder that the exe is ran from.
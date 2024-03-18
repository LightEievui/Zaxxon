# Zaxxon
Recreation of the popular [Zaxxon](https://wikipedia.org/wiki/Zaxxon) Sega arcade game in C++ with the SFML library \
An online playable version of the original Zaxxon rom can be found on [archive.org](https://archive.org/details/arcade_zaxxon).

## How to play
**Controls** \
(PC) - Arrow keys to control player, pitch is inverted. Z to shoot.\
(Arcade) - Stick to control player, pitch is inverted. Button 0 on first controller to shoot.

**Goal** \
Hit as many targets as you can without being hit or running out of fuel, each lap the games gets faster. \
Fuel is regained by destroying the red fuel tanks. \
There are 3 stages, the basic one with just obstacles, space with enemy planes to fight, and the final has a boss. \
High scores are saved and listed, see how high you can get!


# Development

## Building for development
For Visual Studio 2022 (ISO C++ 14 standard): \
Clone the repository to Visual Studio. Set your build to `x86` (32-bit), make sure you are in `Debug` mode for all debug information and build!

## Building for production
Follow development build process to begin. Prefer `Release` mode over `Debug` mode.\
Resulting EXE file must be put in a directory with `openal32.dll` and have a `/res/` path which contains the game assets. \
Other files are statically linked and are not required to be distributed with the program.


## Remixing and forking
You can freely fork and edit this program how you would like according to the license. \
Full coverage documentation is provided in `/doc/`, specifically `/doc/html/index.html` for a pretty way to view the documentation. \

### SFML
To switch out [SFML](https://www.sfml-dev.org/) version, simply replace `lib/SFML` with your new version. No guranteed support for versions aside from SFML 2.6.1 32-bit version.
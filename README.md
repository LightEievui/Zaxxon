![Zaxxon Logo](https://images.launchbox-app.com/cef46ac6-725e-4014-b7fc-0b7b0a144db0.png)

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
You can freely fork and edit this program how you would like according to the license. \
Full documentation is provided in `/doc/`, specifically `/doc/html/index.html` for a pretty way to view the documentation. \
This project was created using Visual Studio 2022 (ISO C++ 14 standard)

## Building for development
Clone the repository to Visual Studio. Your build should automatically be set to x86 (32-bit), make sure you are in `Debug` mode for all debug information. \
If those are set, you can build!

## Building for production
Follow development build process to begin. Prefer `Release` mode over `Debug` mode.\
Resulting EXE file must be put in a directory with `openal32.dll` and have a `/res/` path which contains the game assets. \
Other files are statically linked and are not required to be distributed with the program.

### SFML
To switch out [SFML](https://www.sfml-dev.org/) version, simply replace `lib/SFML` with your new version. \
No guranteed support for versions aside from SFML 2.6.1 32-bit version. \
It is likely slight version changes will be fine, but be prepared to make code alterations if changing the versions gives errors.

### Game resources
All sprites, sounds, and images can be found in the `/res/` path. \
The only unused path within this folder is `/concept/` which contains additional images used to aid in the development of this project. \
Sounds can be gotten from `/sfx/` and can be freely changed out without changing the code. The naming must be the same for the sound you replace. \
Sprites can also be switched out, but as some dimensions are hardcoded into the project, you may also need to find that sprite in the code and manually switch the dimensions.
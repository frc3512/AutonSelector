# AutonSelector

The Autonomous Selector is a program we wrote and a protocol we invented to
facilitate running multiple autonomous modes, each being selectable from the
Driver Station.

## Dependencies

If one is building on Windows, we recommend using the [MSYS2 compiler](https://msys2.github.io/). The following libraries are required as dependencies and should be installed using either your package manager of choice or MSYS2's pacman:

* GCC 7 or newer for some C++17 features
* Qt 5

For MSYS2, use the MINGW64 terminal for building and install the following packages:

* mingw-w64-x86_64-gcc
* mingw-w64-x86_64-qt5

## Building AutonSelector

To build the project, first run `qmake dir` within a terminal from the desired build directory, where "dir" is the relative location of the AutonSelector.pro file. This will generate three makefiles. If a debug build is desired, run `make -f Makefile.Debug`. The default behavior when simply running `make` is to perform a release build.

To cross-compile from Linux to Windows:

1. Install the MinGW-w64 toolchain (`mingw-w64-gcc`)
2. Install a MinGW-w64 build of Qt 5 (`mingw-w64-qt5-base`)
3. Run `publish-linux2win32.sh` to create a .zip of the application binary
   and necessary files

To compile natively on windows:

1. Open MSYS64 terminal provided by MSYS2
2. Install `mingw-w64-x86_64-gcc`, `mingw-w64-x86_64-qt5`, and `zip`
3. Run `publish-msys2win32.sh` to create a .zip of the application binary
   and necessary files

## Robot Setup

To use this program with a new robot, copy the AutonSelector folder in the [host folder](host) into the source tree and `#include AutonSelector.hpp`.

## AutonSelector Client Setup

Since this project was designed to link to libstdc++ and libgcc statically, AutonSelector.exe and IPSettings.txt are the only required files on the Driver Station.

### IPSettings.txt

#### `dsDataPort`

Port on which the AutonSelector receives data from the robot

#### `robotIP`

Robot's IP address

#### `robotDataPort`

Port to which to send connection packets and autonomous mode selections

###### Example IPSettings.txt

    streamHost        = 10.35.12.11
    streamPort        = 80
    streamRequestPath = /mjpg/video.mjpg

    alfCmdPort        = 3512

    #the DS binds to this
    dsDataPort        = 1130

    #the DS sends to this
    robotIP           = 10.35.12.2
    robotDataPort     = 1130

## Usage

### Autonomous Routines

The AutonSelector supports selection of an autonomous mode without needing to rebuild code or reboot the roboRIO. To leverage this functionality, the appropriate autonomous functions must be made available to the AutonSelector.

#### Interface

##### `void AddAutoMethod(std::string_view methodName, std::function<void()> func)`

Add the appropriate autonomous functions to the list available on the AutonSelector.

##### `void DeleteAllMethods()`

Erases the current list of autonomous functions stored.

##### `void ExecAutonomousInit()`

Executes the currently selected autonomous routine.

* Call this in the AutonomousInit() function of the TimedRobot template

##### `void ExecAutonomousPeriodic()`

Executes the currently selected autonomous routine.

* Call this in the AutonomousPeriodic() function of the TimedRobot template

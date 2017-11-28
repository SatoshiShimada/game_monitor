[![Build Status](https://travis-ci.org/SatoshiShimada/game_monitor.svg?branch=master)](https://travis-ci.org/SatoshiShimada/game_monitor)

# Game Monitor

The robot state viewer from robot communication. It's for the robot "Accelite" of team CITBrains at RoboCup Soccer Humanoid League.
If you find a bug please let me know by opening an issue in this repository.  
Please check Wiki for more information.

## Supported OS

* Linux
* Mac OS
* Windows

## Requirements

* [Qt](https://www.qt.io/)

## How to build

### Linux (Ubuntu 14.04)

Install libraries.

```
apt-get install libqt4-core libqt4-dev libqt4-gui qt4-dev-tools
```

If your system is Ubuntu 12.04, run follow command.

```
add-apt-repository ppa:ubuntu-toolchain-r/test
apt-get update
apt-get install gcc-4.8 g++-4.8
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 20
update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 20
```

Finally, compile sources.

```
./qt_make.sh
```

### Mac OS

Install libraries.

```
brew update -all
brew install cartr/qt4/qt
```

Finally, compile sources.

```
./qt_make.sh
```

### Windows

After install requirement: Qt 4.8 and Visual Studio

Run `qt_make.bat` file in Developer Command Prompt window.
[How to open it](https://msdn.microsoft.com/en-us/library/f35ctcxw.aspx "Jump to Microsoft document page")

Or generate Visual Studio project file by CMake:
Use `CMakeLists.txt`

## Author

Satoshi Shimada (mylinux1204@gmail.com)

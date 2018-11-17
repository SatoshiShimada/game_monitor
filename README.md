[![Build Status](https://travis-ci.org/SatoshiShimada/game_monitor.svg?branch=master)](https://travis-ci.org/SatoshiShimada/game_monitor)

# Game Monitor

The robot state viewer via robot communication.  
It is for CIT Brains' robot "Accelite".  
This application supports Linux, Mac OS and Windows.  
If you find a bug please let me know by opening an issue in this repository.  
Please check Wiki for more information.  

## Requirements

* [Qt](https://www.qt.io/) (Version: 5.3 or greater)
* gcc (only Linux)
* XCode (only Mac OS)
* Visual Studio (only Windows)

## How to build

### Linux (Ubuntu 16.04)

1. Install libraries.

```
apt install qt5-default
```

2. Build application.

```
./qt_make.sh
```

### Mac OS

1. Install libraries.

```
brew update -all
brew install qt
```

2. Build application.

```
./qt_make.sh
```

### Windows

1.  Run `qt_make.bat` script in Developer Command Prompt window.  
[How to open](https://msdn.microsoft.com/en-us/library/f35ctcxw.aspx "Jump to Microsoft document page")

Or  

1. Build on Visual Studio using CMake (`CMakeLists.txt`).

## License

MIT License (see `LICENSE` file).


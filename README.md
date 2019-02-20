[![Build Status](https://travis-ci.org/SatoshiShimada/game_monitor.svg?branch=master)](https://travis-ci.org/SatoshiShimada/game_monitor)

# Game Monitor

![Screen Image](https://github.com/SatoshiShimada/game_monitor/wiki/figures/screen_image2.png)

The robot state viewer via robot communication.  
It is for CIT Brains' robot "Accelite".  
This application supports Linux, Mac OS and Windows.  
If you find a bug please let me know by opening an issue in this repository.  
Please check Wiki for more information.  

## Requirements

* [Qt](https://www.qt.io/) (Version: 5.3 or greater)

## How to build

### Linux (Ubuntu 14.04)

1. Install libraries.

```shell
git checkout ubuntu14.04
apt install build-essential
```

This application require Qt 5.3 or greater.
But, Qt version is 5.2 if install using `apt-get`.
You need install Qt from source code.

### Linux (Ubuntu 16.04)

1. Install libraries.

```
apt install build-essential qt5-default qt5-qmake qtbase5-dev-tools qtmultimedia5-dev
```

2. Build application.

```
./qt_make.sh
```

### Linux (Ubuntu 18.04)

1. Install libraries.

```
apt install qt5-default qtmultimedia5-dev
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


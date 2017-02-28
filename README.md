# Game Monitor

The robot state viewer from robot communication. It's for the robot "Accelite" of team CITBrains at RoboCup Soccer Humanoid League.
If you find a bug please let me know by opening an issue in this repository.

## Supported OS

* Linux (Ubuntu)
* Mac OS Sierra

## Requirements

* Qt 4.8
* Boost C++

## How to build

### Linux (Ubuntu 14.04)

Install libraries.

```
apt-get install libqt4-core libqt4-dev libqt4-gui qt4-dev-tools
apt-get install libboost-all-dev
```

If your system is 'Ubuntu 12.04', run follow command.

```
add-apt-repository ppa:ubuntu-toolchain-r/test
apt-get update
apt-get install gcc-4.8 g++-4.8
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 20
update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 20
```

Finally, compile it.

```
./qt_make.sh
```

### Mac OS

Install libraries.

```
brew update -all
brew install qt4
brew install boost
```

Finally, compile it.

```
./qt_make.sh
```

## Author

Satoshi SHIMADA (mylinux1204@gmail.com)


#!/bin/sh

###################
# Requirement
###################
# * g++ (version > 4.8)
# * qt4
# * boost

###################
# HOW TO INSTALL Qt4
# on Ubuntu 14.04
###################
# execute follow command:
#sudo apt-get update && sudo apt-get upgrade
#sudo apt-get install libqt4-core libqt4-dev libqt4-gui qt4-dev-tools

###################
# HOW TO INSTALL g++ version 4.8
# on Ubuntu 12.04
###################
#sudo add-apt-repository ppa:ubuntu-toolchain-r/test
#sudo apt-get update
#sudo apt-get install gcc-4.8 g++-4.8
#sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 20
#sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 20

###################
# HOW TO BUILD on MacOSX
# install qt4 and boost
###################
#brew update -all
#brew install qt4
#brew install boost
#qt_make.sh

PROJECT='GameMonitor.pro'

if [ $(which qmake-qt4) ]; then
	QMAKE='qmake-qt4'
else
	QMAKE='qmake'
fi
echo $QMAKE
$QMAKE -project -o $PROJECT
echo 'QMAKE_CXXFLAGS += --std=c++11' >> $PROJECT
echo 'unix:LIBS += -lboost_system' >> $PROJECT
$QMAKE
make all -j8

if [ ! -d log ]; then
	mkdir log
fi


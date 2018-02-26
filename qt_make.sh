#!/bin/bash -xe

###################
# Requirement
###################
# * g++ (version > 4.8)
# * qt

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
# install qt4
###################
#brew update -all
#brew install qt4

PROJECT='GameMonitor.pro'

QMAKE='qmake'
echo $($QMAKE --version)
$QMAKE -project -o $PROJECT
echo 'QMAKE_CXXFLAGS += --std=c++11' >> $PROJECT
echo 'QT += network widgets multimedia multimediawidgets' >> $PROJECT

if [ ! -d build ]; then
	mkdir build
fi
echo 'DESTDIR = build' >> $PROJECT
echo 'OBJECTS_DIR = build/obj' >> $PROJECT
echo 'MOC_DIR = build/moc' >> $PROJECT
echo 'RCC_DIR = build/rcc' >> $PROJECT
echo 'UI_DIR = build/ui' >> $PROJECT

$QMAKE
make all -j8

if [ ! -d log ]; then
	mkdir log
fi


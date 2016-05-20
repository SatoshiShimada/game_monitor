#!/bin/sh

# Requirement
# * g++ (version > 4.8)
# * qt4

###################
# HOW TO INSTALL Qt4
# on Ubuntu 14.04
###################
# execute follow command:
#sudo apt-get update && sudo apt-get upgrade
#sudo apt-get install libqt4-core libqt4-dev libqt4-gui qt4-dev-tools

# INSTALL g++ version 4.8
# on Ubuntu 12.04
#sudo add-apt-repository ppa:ubuntu-toolchain-r/test
#sudo apt-get update
#sudo apt-get install gcc-4.8 g++-4.8
#sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 20
#sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 20

PROJECT='GameMonitor.pro'

qmake-qt4 -project -o $PROJECT
echo 'QMAKE_CXXFLAGS += --std=c++11' >> $PROJECT
echo 'unix:LIBS += -lboost_system' >> $PROJECT
qmake-qt4
make all -j4


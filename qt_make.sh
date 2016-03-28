#!/bin/sh

###################
# HOW TO INSTALL Qt4
# on Ubuntu 14.04
###################
# execute follow command:
#sudo apt-get update && sudo apt-get upgrade
#sudo apt-get install libqt4-core libqt4-dev libqt4-gui qt4-dev-tools

PROJECT='GameMonitor.pro'

qmake-qt4 -project -o $PROJECT
echo 'QMAKE_CXXFLAGS += --std=c++11' >> $PROJECT
echo 'unix:LIBS += -lboost_system' >> $PROJECT
qmake-qt4
make all -j4


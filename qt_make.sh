#!/bin/sh

###################
# HOW TO INSTALL
# on Ubuntu 14.04
###################
# execute follow command:
#sudo apt-get install libqt4-core libqt4-dev libqt4-gui qt4-dev-tools

if [ -z $1 ] ;then
	echo "argument too few."
	exit 1
fi

case "$1" in
	"qmake" )
		#qmake -project
		#qmake

		qmake-qt4 -project
		qmake-qt4
		;;
	"make" )
		make
		mv ./interface ../exe
		;;
esac

exit 0


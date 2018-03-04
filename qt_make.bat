
qmake -project -o game_monitor.pro
qmake "QT += network widgets multimedia meltimediawidgets"
nmake release
copy release\game_monitor.exe .\
mkdir log

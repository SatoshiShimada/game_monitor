cmake_minimum_required(VERSION 2.8)

project(game_monitor)

set(SRCS
	src/interface.cpp
	src/interface.h
	src/log.cpp
	src/log.h
	src/main.cpp
	src/pos_types.h
	src/udp_thread.cpp
	src/udp_thread.h
)

set(MOC_HEADERS
	src/interface.h
	src/udp_thread.h
)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
add_definitions(-DBOOST_ALL_NO_LIB)
include(FindSDL)
find_package(SDL REQUIRED)
if(NOT_SDL_FOUND)
    message(FATAL ERROR "SDL not found!!!")
endif(NOT_SDL_FOUND)

find_package(Boost COMPONENTS system)
find_package(Qt4 REQUIRED)

set(QT_USE_QTMAIN true)
set(QT_USE_QTGUI true)
set(QT_USE_QTNETWORK true)
set(QT_USE_QTOPENGL true)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
include_directories(${CMAKE_BINARY_DIR})

QT4_ADD_RESOURCES(RES_SOURCES ${RESOURCES})
QT4_WRAP_CPP(MOC_SRCS ${MOC_HEADERS})

set(CMAKE_BUILD_TYPE Debug)

include_directories(src)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(game_monitor ${SRCS} ${MOC_SRCS} ${RES_SORUCES})
target_link_libraries(game_monitor ${Boost_LIBRARIES})
target_link_libraries(game_monitor ${QT_LIBRARIES})

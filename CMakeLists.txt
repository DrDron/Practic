cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(gensin VERSION 1.0.0 LANGUAGES CXX)
set (CMAKE_PREFIX_PATH "C:\\Qt\\6.0.3\\mingw81_64\\")

set(SOURCES 
	src/gensin.cpp
	#src/mainwindow.cpp
	#src/mainwindow.ui
	#src/resources.qrc
)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets)
add_subdirectory(argparse)
target_link_libraries(${PROJECT_NAME} argparse)

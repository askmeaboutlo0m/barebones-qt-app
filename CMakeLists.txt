# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.18)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
project(barebones LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    message(FATAL_ERROR
        "Required build type missing, run cmake with one of these options:\n"
        "* -DCMAKE_BUILD_TYPE=Debug\n"
        "* -DCMAKE_BUILD_TYPE=Release\n"
        "* -DCMAKE_BUILD_TYPE=RelWithDebInfo\n"
        "* -DCMAKE_BUILD_TYPE=MinSizeRel")
endif()

set(USE_QT "6" CACHE STRING "Qt version to use (5 or 6)")

if("${USE_QT}" STREQUAL "5")
    find_package(QT 5.15 NAMES Qt5 REQUIRED)
elseif("${USE_QT}" STREQUAL "6")
    find_package(QT 6.4 NAMES Qt6 REQUIRED)
else()
    message(FATAL_ERROR "Invalid USE_QT value '${USE_QT}'")
endif()
message(STATUS "Using Qt version ${QT_VERSION_MAJOR}")
find_package("Qt${QT_VERSION_MAJOR}" REQUIRED COMPONENTS Core Gui Sql Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOMOC_PATH_PREFIX ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(${QT_VERSION_MAJOR} EQUAL 5)
    add_executable(barebones)
else()
    qt6_add_executable(barebones WIN32 MACOSX_BUNDLE)
endif()

target_sources(barebones PRIVATE main.cpp)
target_link_libraries(barebones PUBLIC
    "Qt${QT_VERSION_MAJOR}::Core"
    "Qt${QT_VERSION_MAJOR}::Gui"
    "Qt${QT_VERSION_MAJOR}::Widgets"
)

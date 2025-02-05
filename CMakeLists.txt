cmake_minimum_required(VERSION 3.30)
project(XArc)

set(CMAKE_CXX_STANDARD 17)

add_executable(XArc src/main.cpp
        tests/units/testParserCLI.cpp
        src/molules/inc/XArc.h
        src/molules/Settings.cpp
)

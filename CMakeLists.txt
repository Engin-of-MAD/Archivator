cmake_minimum_required(VERSION 3.30)
project(archivator)

set(CMAKE_CXX_STANDARD 17)

add_executable(archivator main.cpp
        molules/compressor.cpp
        tests/units/testParserCLI.cpp
        molules/ParserCLI.cpp
        molules/inc/ParserCLI.h)

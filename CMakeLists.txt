cmake_minimum_required(VERSION 3.30)
project(XArc)


find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Wpedantic)
set(sources
        src/modules/inc/XArc.h
        src/modules/Settings.cpp
)
set(test_libs GTest::GTest GTest::Main)
set(entry_point src/main.cpp)
add_executable(XArc  ${entry_point} ${sources})

add_executable(unit_tests tests/units/testParserCLI.cpp ${sorces})
target_link_libraries(unit_tests ${test_libs})
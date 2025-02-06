cmake_minimum_required(VERSION 3.30)
project(XArc)


find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(SANITIZER_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SANITIZER_FLAGS}")


add_compile_options(-Wall -Wextra -Wpedantic -fsanitize=address)
set(sources
        src/modules/inc/XArc.h
        src/modules/Settings.cpp
)
set(test_libs GTest::GTest GTest::Main)
set(entry_point src/main.cpp)
add_executable(XArc  ${entry_point} ${sources})

add_executable(unit_tests tests/units/testParserCLI.cpp ${sorces})
target_link_libraries(unit_tests ${test_libs})
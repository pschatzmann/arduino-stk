cmake_minimum_required(VERSION 3.16)

# set the project name
project(arduino-stk)

# lots of warnings and all warnings as errors
# add_compile_options(-Wall -Wextra )
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# Build with Linux Arduino Emulator
FetchContent_Declare(arduino_emulator GIT_REPOSITORY "https://github.com/pschatzmann/Arduino-Emulator.git" GIT_TAG main )
FetchContent_GetProperties(arduino_emulator)
if(NOT arduino_emulator_POPULATED)
    FetchContent_Populate(arduino_emulator)
    add_subdirectory(${arduino_emulator_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/arduino_emulator)
endif()

# define source files
file(GLOB_RECURSE SRC_LIST_CPP CONFIGURE_DEPENDS  "${PROJECT_SOURCE_DIR}/src/*.cpp" )

# define libraries
add_library (arduino-stk ${SRC_LIST_CPP})

target_compile_definitions(arduino-stk PUBLIC -DIS_DESKTOP )

# define location for header files
target_include_directories(arduino-stk PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src )

target_link_libraries(arduino-stk  arduino_emulator)

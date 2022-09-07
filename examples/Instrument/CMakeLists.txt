cmake_minimum_required(VERSION 3.20)

# set the project name
project(Instrument)
set (CMAKE_CXX_STANDARD 11)
set (DCMAKE_CXX_FLAGS "-Werror")

include(FetchContent)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# Build with arduino-audio-tools
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../.. ${CMAKE_CURRENT_BINARY_DIR}/arduino-stk )
endif()

# build sketch as executable
set_source_files_properties(Instrument.ino PROPERTIES LANGUAGE CXX)
add_executable (Instrument Instrument.ino)

# set preprocessor defines
target_compile_definitions(arduino_emulator PUBLIC -DDEFINE_MAIN)
target_compile_definitions(Instrument PUBLIC -DARDUINO -DIS_DESKTOP -DEXIT_ON_STOP)

# OS/X might need this setting for core audio
#target_compile_definitions(portaudio PUBLIC -DPA_USE_COREAUDIO=1)

# specify libraries
target_link_libraries(Instrument arduino_stk arduino_emulator)

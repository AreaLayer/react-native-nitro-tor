cmake_minimum_required(VERSION 3.9.0)
project(NitroTor)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 20)

# Define paths
set(CPP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../cpp")
set(ANDROID_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(LIBS_DIR "${ANDROID_DIR}/src/main/jniLibs/${ANDROID_ABI}")

# Check for required files
set(TOR_LIB "${LIBS_DIR}/libtor.a")

if(NOT EXISTS "${TOR_LIB}")
    message(FATAL_ERROR "libtor.a not found at: ${TOR_LIB}")
endif()

message(STATUS "Using libraries from: ${LIBS_DIR}")

# Create the main library
add_library(${PROJECT_NAME}
    SHARED
    ${CPP_DIR}/lib.rs.cc
    ${CPP_DIR}/glob_impl.cpp
    ${ANDROID_DIR}/src/main/cpp/cpp-adapter.cpp
)

# Include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CPP_DIR}
    ${ANDROID_DIR}/src/main/cpp
    ${LIBS_DIR}
)

# Find Android log library
find_library(LOG_LIB log)

# Create imported targets for external libraries
add_library(tor_lib SHARED IMPORTED)
set_target_properties(tor_lib PROPERTIES
    IMPORTED_LOCATION "${TOR_LIB}"
    IMPORTED_NO_SONAME TRUE
)

# Additional compiler flags
target_compile_options(${PROJECT_NAME}
    PRIVATE
    -fexceptions
    -frtti
)

# Include autolinking
include(${CMAKE_SOURCE_DIR}/../nitrogen/generated/android/NitroTor+autolinking.cmake)

# Add our additional libraries after autolinking
target_link_libraries(
    ${PROJECT_NAME}
    ${LOG_LIB}
    # ${CXXBRIDGE_LIB}
    ${TOR_LIB}
    dl
    m
    android
)
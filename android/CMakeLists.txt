cmake_minimum_required(VERSION 3.9.0)
project(NitroTor)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 20)

# Define paths
set(CPP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../cpp")
set(ANDROID_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(LIBS_DIR "${ANDROID_DIR}/src/main/jniLibs/${ANDROID_ABI}")

# Check for required files
set(TOR_LIB "${LIBS_DIR}/libtor.so")
set(CXX_LIB "${LIBS_DIR}/libcxxbridge1.a")


if(NOT EXISTS "${TOR_LIB}")
    message(FATAL_ERROR "libtor.so not found at: ${TOR_LIB}")
endif()

if(NOT EXISTS "${CXX_LIB}")
    message(FATAL_ERROR "libcxxbridge1.so not found at: ${CXX_LIB}")
endif()

# Create the main library
add_library(${PROJECT_NAME}
    SHARED
    # ${CPP_DIR}/HybridTor.cpp
    ${CPP_DIR}/lib.rs.cc
    ${CPP_DIR}/HybridMath.cpp
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

# Create imported target for libtor
add_library(tor_lib SHARED IMPORTED)
set_target_properties(tor_lib PROPERTIES
    IMPORTED_LOCATION "${TOR_LIB}"
    IMPORTED_NO_SONAME TRUE
)

# Include autolinking before linking (important!)
include(${CMAKE_SOURCE_DIR}/../nitrogen/generated/android/NitroTor+autolinking.cmake)

# Link libraries using plain syntax to match autolinking.cmake
target_link_libraries(${PROJECT_NAME} ${LOG_LIB} ${CXX_LIB} ${TOR_LIB} android)

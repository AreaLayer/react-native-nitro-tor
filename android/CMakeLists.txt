cmake_minimum_required(VERSION 3.9.0)
set(PACKAGE_NAME NitroTor)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 20)

# Path to jniLibs where you copied the Rust .so files
set(RUST_JNI_LIBS_DIR "${CMAKE_SOURCE_DIR}/src/main/jniLibs")

# Define C++ library and add all sources
add_library(${PACKAGE_NAME} SHARED
    src/main/cpp/cpp-adapter.cpp
    ../cpp/HybridMath.cpp
    ../cpp/HybridTor.cpp
)

# Add Nitrogen specs (assuming this part is correct for Nitro Modules)
include(${CMAKE_SOURCE_DIR}/../nitrogen/generated/android/NitroTor+autolinking.cmake)

# Set up local includes
include_directories(
    "src/main/cpp"
    "../cpp"
    "${RUST_JNI_LIBS_DIR}/${ANDROID_ABI}" # Include path to jniLibs for current ABI
)

find_library(LOG_LIB log)

# Link against libraries
target_link_libraries(
    ${PACKAGE_NAME}
    ${LOG_LIB}
    # Link to the Rust .so library from jniLibs
    IMPORTED_RUST_LIB # Using an IMPORTED target (see below)
)

# --- IMPORTED Target for Rust .so Library ---
# Define an IMPORTED library target to represent the Rust .so
add_library(IMPORTED_RUST_LIB SHARED IMPORTED)

# Set the location of the IMPORTED Rust library for each architecture
set_target_properties(IMPORTED_RUST_LIB PROPERTIES
    IMPORTED_LOCATION "${RUST_JNI_LIBS_DIR}/${ANDROID_ABI}/libtor.so" # <--- VERIFY RUST .SO FILENAME!
)


# Optional: Copy C++ .so to jniLibs (if needed, review Nitro Modules build process)
# add_custom_command(TARGET_OUTPUTS ${PACKAGE_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         $<TARGET_FILE:${PACKAGE_NAME}>
#         ${RUST_JNI_LIBS_DIR}/${ANDROID_ABI}/lib${PACKAGE_NAME}.so
#     DEPENDS $<TARGET_FILE:${PACKAGE_NAME}>
# )
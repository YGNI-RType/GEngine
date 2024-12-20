# define_compilers.cmake

# All plateforms (dict of sources) : https://stackoverflow.com/questions/70475665/what-are-the-possible-values-of-cmake-system-processor
# Windows : https://superuser.com/questions/305901/possible-values-of-processor-architecture
# Linux : https://stackoverflow.com/questions/45125516/possible-values-for-uname-m/45125525#45125525

# Detect the architecture if not already defined
if(NOT DEFINED ARCHITECTURE)
    if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
        set(ARCHITECTURE "x64")
    elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "AMD64")
        set(ARCHITECTURE "x64")
    elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "aarch64")
        set(ARCHITECTURE "arm64")
    elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "arm64")
        set(ARCHITECTURE "arm64")
    elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "ARM64")
        set(ARCHITECTURE "arm64")
    elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "i386")
        set(ARCHITECTURE "x86")
    elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "armv8l")
        set(ARCHITECTURE "arm64")
    elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "i686")
        set(ARCHITECTURE "x86")
    elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "armv7l")
        set(ARCHITECTURE "arm32")
    else()
        message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
    endif()
endif()

# VCPKG
if (ARCHITECTURE STREQUAL "x64")
    set(VCPKG_TARGET_ARCHITECTURE "x64")
elseif (ARCHITECTURE STREQUAL "x86")
    set(VCPKG_TARGET_ARCHITECTURE "x86")
elseif (ARCHITECTURE STREQUAL "arm64")
    set(VCPKG_TARGET_ARCHITECTURE "arm64")
elseif (ARCHITECTURE STREQUAL "arm32")
    set(VCPKG_TARGET_ARCHITECTURE "arm")
endif()

# https://stackoverflow.com/questions/9160335/os-specific-instructions-in-cmake-how-to
if(LINUX)
    if(ARCHITECTURE STREQUAL "x64")
        # include(${CMAKE_SOURCE_DIR}/cmake/compilers/GNU/x64.cmake)
        set(VCPKG_TRIPLET "x64-linux")
    elseif(ARCHITECTURE STREQUAL "x86")
        include(${CMAKE_SOURCE_DIR}/cmake/compilers/GNU/x86.cmake)
        set(VCPKG_TRIPLET "x86-linux")
    elseif(ARCHITECTURE STREQUAL "arm64")
        include(${CMAKE_SOURCE_DIR}/cmake/compilers/GNU/armv8.cmake)
        set(VCPKG_TRIPLET "arm64-linux")
    elseif(ARCHITECTURE STREQUAL "arm32")
        include(${CMAKE_SOURCE_DIR}/cmake/compilers/GNU/armv7.cmake)
        set(VCPKG_TRIPLET "arm-linux")
    else()
        message(FATAL_ERROR "Unsupported architecture: ${ARCHITECTURE} for OS: ${CMAKE_SYSTEM_NAME}")
    endif()
elseif(WIN32)
    if(ARCHITECTURE STREQUAL "x64")
        # include(${CMAKE_SOURCE_DIR}/cmake/compilers/Windows/x64.cmake)
        set(VCPKG_TRIPLET "x64-windows")
    elseif(ARCHITECTURE STREQUAL "x86")
        include(${CMAKE_SOURCE_DIR}/cmake/compilers/Windows/x86.cmake)
        set(VCPKG_TRIPLET "x86-windows")
    elseif(ARCHITECTURE STREQUAL "arm64")
        include(${CMAKE_SOURCE_DIR}/cmake/compilers/Windows/armv8.cmake)
        set(VCPKG_TRIPLET "arm64-windows")
    elseif(ARCHITECTURE STREQUAL "arm32")
        include(${CMAKE_SOURCE_DIR}/cmake/compilers/Windows/armv7.cmake)
        set(VCPKG_TRIPLET "arm-windows")
    else()
        message(FATAL_ERROR "Unsupported architecture: ${ARCHITECTURE} for OS: ${CMAKE_SYSTEM_NAME}")
    endif()
elseif(APPLE)
    if(ARCHITECTURE STREQUAL "x64")
        include(${CMAKE_CURRENT_LIST_DIR}/compilers/Apple/x64.cmake)
        set(VCPKG_TRIPLET "x64-osx")
    elseif(ARCHITECTURE STREQUAL "arm64")
        include(${CMAKE_CURRENT_LIST_DIR}/compilers/Apple/armv8.cmake)
        set(VCPKG_TRIPLET "arm64-osx")
    else()
        message(FATAL_ERROR "Unsupported architecture: ${ARCHITECTURE} for OS: ${CMAKE_SYSTEM_NAME}")
    endif()
else()
    message(FATAL_ERROR "Unsupported OS: ${CMAKE_SYSTEM_NAME}")
endif()

set(VCPKG_TARGET_TRIPLET "${VCPKG_TRIPLET}")

message(STATUS "VCPKG_TARGET_ARCHITECTURE: ${VCPKG_TARGET_ARCHITECTURE} ARCHITECTURE: ${ARCHITECTURE} Triplet : ${VCPKG_TRIPLET}")

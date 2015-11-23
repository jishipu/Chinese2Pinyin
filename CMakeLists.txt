cmake_minimum_required(VERSION 2.8.8)

project(C2P)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CHINESE2PINYIN_DIR ${CMAKE_INSTALL_PREFIX}/share/Chinese2Pinyin)

# Silence CMake 3.0 warnings
if(POLICY CMP0043)
    cmake_policy(SET CMP0043 OLD)
endif()

# Set version
set(C2P_VERSION_MAJOR 0)
set(C2P_VERSION_MINOR 13)
set(C2P_VERSION_PATCH 0)
set(C2P_VERSION_STRING "${C2P_VERSION_MAJOR}.${C2P_VERSION_MINOR}.${C2P_VERSION_PATCH}")

# Set up packaging
set(CPACK_PACKAGE_NAME "c2p")
set(CPACK_PACKAGE_VERSION "${C2P_VERSION_STRING}")
set(CPACK_GENERATOR "TGZ")
set(CPACK_SET_DESTDIR FALSE)
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_SOURCE_IGNORE_FILES "/build/;/.git;/*.user;/.tx/;~$;${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)
add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)

# Definitions
add_definitions(-Wall)
add_definitions("-DCHINESE2PINYIN_DIR=\"${CHINESE2PINYIN_DIR}\"")

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Handle build type
if(CMAKE_BUILD_TYPE MATCHES [Dd]ebug)
    message(STATUS "Debug build")
    add_definitions(-DDEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
else()
    message(STATUS "Release build")
    add_definitions(-DNDEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

# Standard installation paths
include(GNUInstallDirs)

# Macros
include(FeatureSummary)

# Qt 5
find_package(Qt5Core REQUIRED)

# Add subdirectories
add_subdirectory(test)

# Display feature summary
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

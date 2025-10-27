cmake_minimum_required(VERSION 3.22)

set(APPLICATION_NAME "profile")

project(${APPLICATION_NAME})

include(FindPkgConfig)
include(CheckLibraryExists)

# Enable AddressSanitizer
set(ASAN_ENABLED TRUE)

set(CMAKE_COLOR_MAKEFILE ON)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "=== RELEASE MODE ===")
    add_definitions("-DNDEBUG")
    add_definitions("-Wall -O3")
else()
    message(STATUS "=== DEBUG MODE ===")
    add_definitions("-DDEBUG")
    add_definitions("-Wall -Wextra -pedantic -O0 -g")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")

# additional compiler flags
set(CMAKE_COLOR_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(
    ${PROJECT_SOURCE_DIR}
)

# link_directories(
# )

set( SOURCES
    "test.cpp"
)

# link_libraries (
# )

add_executable(
    ${APPLICATION_NAME}
    ${SOURCES}
)


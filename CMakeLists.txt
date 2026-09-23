cmake_minimum_required(VERSION 3.24...3.30)

project(CapitalAlien LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# =========================
# Raylib
# =========================

set(RAYLIB_VERSION 6.0)

FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
)

FetchContent_MakeAvailable(raylib)

# =========================
# Project files
# =========================

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.cpp"
)

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.h"
)

# =========================
# Executable
# =========================

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

# =========================
# Include directory
# =========================

target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/headers"
)

# =========================
# Raylib
# =========================

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        raylib
)
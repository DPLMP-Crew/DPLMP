cmake_minimum_required(VERSION 3.30)
project(DPLMP LANGUAGES C CXX)
set(FETCHCONTENT_QUIET OFF CACHE BOOL "Verbose FetchContent" FORCE)

option(DPLMP_BUILD_CLIENT "Build Client Binary" ON)
option(DPLMP_BUILD_SERVER "Build Server Binary" ON)

include(FetchContent)

FetchContent_Declare(
    safetyhook
    GIT_REPOSITORY "https://github.com/cursey/safetyhook.git"
    GIT_TAG "main"
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(safetyhook)

FetchContent_Declare(
    glm
    GIT_REPOSITORY "https://github.com/g-truc/glm.git"
    GIT_TAG "master"
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    lua
    GIT_REPOSITORY "https://github.com/marovira/lua.git"
    GIT_TAG "master"
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(lua)

FetchContent_Declare(
    sol2
    GIT_REPOSITORY "https://github.com/ThePhD/sol2.git"
    GIT_TAG "main"
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(sol2)

FetchContent_Declare(
    xxhash
    GIT_REPOSITORY "https://github.com/Cyan4973/xxHash.git"
    GIT_TAG "cmake"
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(xxhash)

add_library(xxHashStatic STATIC
    ${xxhash_SOURCE_DIR}/xxhash.c
)

target_include_directories(xxHashStatic PUBLIC
    ${xxhash_SOURCE_DIR}
)

FetchContent_Declare(
    RakNet
    GIT_REPOSITORY "https://github.com/facebookarchive/RakNet.git"
    GIT_TAG "origin/master"
)
FetchContent_MakeAvailable(RakNet)

add_subdirectory(Shared)

if (WIN32 AND DPLMP_BUILD_CLIENT)
    add_subdirectory(Client)
endif()

if (DPLMP_BUILD_SERVER)
    add_subdirectory(Server)
endif()
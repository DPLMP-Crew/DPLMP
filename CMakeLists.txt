cmake_minimum_required(VERSION 3.30)

project(DPLMP VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(FETCHCONTENT_QUIET OFF CACHE BOOL "Verbose FetchContent" FORCE)

set(ENET_TEST OFF CACHE BOOL "Build enet tests" FORCE)

option(DPLMP_BUILD_CLIENT "Build Client Binary" ON)
option(DPLMP_BUILD_SERVER "Build Server Binary" ON)
option(DPLMP_USE_SAFETYHOOK "Use SafetyHook isntead of MinHook" OFF)

include(FetchContent)

if(WIN32 AND DPLMP_BUILD_CLIENT)

if(DPLMP_USE_SAFETYHOOK)
FetchContent_Declare(
    safetyhook
    GIT_REPOSITORY "https://github.com/cursey/safetyhook.git"
    GIT_TAG 983ba5c4b72866c8ed5020b6d57b7108fd1622c2
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(safetyhook)
else()
FetchContent_Declare(
    MinHook
    GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
    GIT_TAG 1e9ad1eb42db11bfcb65461f687c656612d1b555
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(MinHook)
endif()

FetchContent_Declare(
    glm
    GIT_REPOSITORY "https://github.com/g-truc/glm.git"
    GIT_TAG e7970a8b26732f1b0df9690f7180546f8c30e48e
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    lua
    GIT_REPOSITORY "https://github.com/marovira/lua.git"
    GIT_TAG 4737fb98d04a3e2fd365e55d8cb95e4c961db3a5
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(lua)

FetchContent_Declare(
    sol2
    GIT_REPOSITORY "https://github.com/ThePhD/sol2.git"
    GIT_TAG c1f95a773c6f8f4fde8ca3efe872e7286afe4444
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(sol2)

FetchContent_Declare(
    xxhash
    GIT_REPOSITORY "https://github.com/Cyan4973/xxHash.git"
    GIT_TAG 68ae745162c644dfbf2e3adec4bc8ae9592d9ba9
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
    enet
    GIT_REPOSITORY "https://github.com/zpl-c/enet.git"
    GIT_TAG 8647b6eaea881c86471ae29f732620d299fc20d7
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(enet)

add_library(ENetStatic INTERFACE)

target_include_directories(ENetStatic INTERFACE
    ${enet_SOURCE_DIR}/include
)

endif()

add_subdirectory(Shared)

if (WIN32 AND DPLMP_BUILD_CLIENT)
    add_subdirectory(Client)
endif()

if (DPLMP_BUILD_SERVER)
    add_subdirectory(Server)
endif()


set(CPACK_PACKAGE_VENDOR "DPLMP-Crew")

include(CPack)
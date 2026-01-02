cmake_minimum_required(VERSION 3.21)
project(DPLMP LANGUAGES C CXX)
set(FETCHCONTENT_QUIET OFF CACHE BOOL "Verbose FetchContent" FORCE)

include(FetchContent)

FetchContent_Declare(
    safetyhook
    GIT_REPOSITORY "https://github.com/cursey/safetyhook.git"
    GIT_TAG "origin/main"
)
FetchContent_MakeAvailable(safetyhook)

#FetchContent_Declare(
#    RakNet
#    GIT_REPOSITORY "https://github.com/facebookarchive/RakNet.git"
#    GIT_TAG "origin/master"
#)
#FetchContent_MakeAvailable(RakNet)

add_subdirectory(Client)
# TODO
# add_subdirectory(Shared)
# add_subdirectory(Server)
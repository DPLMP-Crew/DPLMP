cmake_minimum_required(VERSION 3.21)
project(DPLMP LANGUAGES C CXX)

include(FetchContent)

FetchContent_Declare(
    safetyhook
    GIT_REPOSITORY "https://github.com/cursey/safetyhook.git"
    GIT_TAG "origin/main"
)
FetchContent_MakeAvailable(safetyhook)

add_subdirectory(Client)
# TODO
# add_subdirectory(Shared)
# add_subdirectory(Server)
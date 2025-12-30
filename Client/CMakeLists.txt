add_library(DPLMPClient SHARED)

target_sources(DPLMPClient
    PRIVATE
        src/dllmain.cpp
)

target_include_directories(DPLMPClient
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
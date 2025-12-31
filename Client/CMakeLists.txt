add_library(DPLMPClient SHARED)

target_compile_definitions(DPLMPClient
    PRIVATE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
)

target_sources(DPLMPClient
    PRIVATE
        src/dllmain.cpp
        src/Core.cpp
)

target_include_directories(DPLMPClient
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(DPLMPClient PRIVATE safetyhook)
add_executable(Server)

target_sources(Server
    PRIVATE
        src/main.cpp
)

target_link_libraries(Server
    PRIVATE
        Shared
)

if (WIN32)
    target_compile_definitions(Server PRIVATE PLATFORM_WINDOWS)
elseif (UNIX)
    target_compile_definitions(Server PRIVATE PLATFORM_LINUX)
endif()

add_executable(Server)

target_sources(Server
    PRIVATE
        src/main.cpp
        src/Server.cpp
)

target_include_directories(Server
    PRIVATE
	    ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Server
    PRIVATE
        enet_static
        Shared
)

if (WIN32)
    target_compile_definitions(Server PRIVATE PLATFORM_WINDOWS)
    target_compile_definitions(Shared
    PRIVATE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
        PLATFORM_WINDOWS
    )
    set(CPACK_GENERATOR "ZIP")
elseif (UNIX)
    target_compile_definitions(Server PRIVATE PLATFORM_LINUX)
    target_compile_definitions(Shared PRIVATE PLATFORM_LINUX)
    set(CPACK_GENERATOR "TGZ")
endif()

install(CODE [[
  file(REMOVE_RECURSE "${CMAKE_INSTALL_PREFIX}")
]])

install(TARGETS Server
    RUNTIME DESTINATION .
)
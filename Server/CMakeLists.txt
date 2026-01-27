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
        Shared
)

if (WIN32)
    target_compile_definitions(Server PRIVATE PLATFORM_WINDOWS)
    set(CPACK_GENERATOR "ZIP")
elseif (UNIX)
    target_compile_definitions(Server PRIVATE PLATFORM_LINUX)
    set(CPACK_GENERATOR "TGZ")
endif()

install(CODE [[
  file(REMOVE_RECURSE "${CMAKE_INSTALL_PREFIX}")
]])

install(TARGETS Server
    RUNTIME DESTINATION .
)

set(CPACK_PACKAGE_NAME "Server")
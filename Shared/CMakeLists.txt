add_library(Shared STATIC)

target_sources(Shared
    PRIVATE
        src/placeholder.cpp
)

target_include_directories(Shared
    PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
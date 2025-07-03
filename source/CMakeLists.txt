
#====================================================================

# Output
set(BINARY ${PROJECT_NAME})

# 3rd party
include(${CMAKE_SOURCE_DIR}/cmake/repo/v8.cmake)

# Source files
file(GLOB_RECURSE ${BINARY}_SOURCES LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/cpp/*.cpp")

# Add library
add_library(${BINARY} ${${BINARY}_SOURCES})

# Shared or Static
if(BUILD_SHARED_LIBS)
    target_compile_definitions(${BINARY} PRIVATE C8_SHARED)
else()
    target_compile_definitions(${BINARY} PRIVATE C8_STATIC)
endif()

# Include directories
target_include_directories(${BINARY} PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Precompiled headers
target_precompile_headers(${BINARY} PUBLIC ${CMAKE_SOURCE_DIR}/include/c8/c8-internal.h)

# Link libraries
target_link_libraries(${BINARY} Boost::stacktrace_backtrace v8)

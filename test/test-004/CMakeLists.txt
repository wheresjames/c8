
#====================================================================

# Output
get_filename_component(TEST_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(BINARY ${TEST_NAME})

# Ensure debug symbols are included
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")

# 3rd party
include(${CMAKE_SOURCE_DIR}/cmake/repo/boost.cmake)

# Source files
file(GLOB_RECURSE ${BINARY}_SOURCES LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Add library
add_executable(${BINARY} ${${BINARY}_SOURCES})

# Include directories
target_include_directories(${BINARY} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${BINARY} PUBLIC ${v8_INCLUDE_DIRS})

# Link directories
target_link_directories(${BINARY} PUBLIC ${v8_BINARY_DIR})

# Link libraries
target_link_libraries(${BINARY} PRIVATE c8)

# Strip debug symbols into a separate file
strip_symbols_postbuild(${BINARY})

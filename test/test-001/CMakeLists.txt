
#====================================================================

# Output
get_filename_component(TEST_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(BINARY ${TEST_NAME})

# Source files
file(GLOB_RECURSE ${BINARY}_SOURCES LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Add exe target
add_executable(${BINARY} ${${BINARY}_SOURCES})

# Include directories
target_include_directories(${BINARY} PUBLIC ${CMAKE_SOURCE_DIR}/include)

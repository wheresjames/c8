
#====================================================================

# Add all subdirectories that start with "test-"
file(GLOB TEST_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/test-*)
foreach(TEST_DIR ${TEST_DIRS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_DIR})
        message(STATUS "Adding test directory: ${TEST_DIR}")
        add_subdirectory(${TEST_DIR})
    endif()
endforeach()


cmake_minimum_required(VERSION 3.24)

#-------------------------------------------------------------
# Options
set(CMAKE_CXX_STANDARD 20)
set(FETCHCONTENT_QUIET FALSE)

# Release or Debug
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()


#-------------------------------------------------------------
# Helper functions
include(${CMAKE_SOURCE_DIR}/cmake/debug.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/readProjectConfig.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/getTargetBuildPath.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/getLatestStableGitTag.cmake)

# Show compiler info
message(STATUS " [-] CMAKE_C_COMPILER=${CMAKE_C_COMPILER}")
message(STATUS " [-] CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
message(STATUS " [-] CMAKE_LINKER=${CMAKE_LINKER}")


#-------------------------------------------------------------
# Read project config
readProjectConfig("${CMAKE_SOURCE_DIR}/PROJECT.txt")
dump_cmake_variables(APP)
project("${APPNAME}"
        VERSION "${APPVER}"
        DESCRIPTION "${APPDESC}"
        HOMEPAGE_URL "${APPURL}"
        LANGUAGES CXX
    )

# App Variables
add_compile_options(-DAPPNAME="${APPNAME}")
add_compile_options(-DAPPNAMERAW=${APPNAME})
add_compile_options(-DAPPVERSION="${APPVERSION}")
add_compile_options(-DAPPBUILD="${APPBUILD}")
add_compile_options(-DAPPDESCRIPTION="${APPDESCRIPTION}")
add_compile_options(-DAPPURL="${APPURL}")


#-------------------------------------------------------------
# Versions

#set(LIB3_V8_VERSION "13.6.236")
get_latest_stable_git_tag("https://github.com/v8/v8.git" LIB3_V8_VERSION)

dump_cmake_variables(LIB3)


#-------------------------------------------------------------
# 3rd party libs
get_target_build_path(LIB3_BUILD_NAME)
set(LIB3_BUILD_DIR "${CMAKE_SOURCE_DIR}/lib3/${LIB3_BUILD_NAME}")
message(STATUS "\n-- LIB3_BUILD_DIR=${LIB3_BUILD_DIR}")


#-------------------------------------------------------------
# Add code
add_subdirectory(source)
add_subdirectory(test)


#-------------------------------------------------------------
# Install library and headers
install(TARGETS ${APPNAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h"
    )



#====================================================================
# The dream

# include(FetchContent)

# FetchContent_Declare(
#         v8
#         GIT_REPOSITORY  https://github.com/v8/v8.git
#         GIT_TAG         ${v8_VERSION}
#         GIT_PROGRESS    TRUE
# )
# FetchContent_MakeAvailable(v8)


#====================================================================
# The reality

set(v8_ARCH "${CMAKE_SYSTEM_PROCESSOR}")
if (v8_ARCH MATCHES "^x86_64")
    set(v8_ARCH "x64")
endif()
set(v8_ROOT "${LIB3_BUILD_DIR}/v8/${LIB3_V8_VERSION}")
set(v8_SOURCE_DIR "${v8_ROOT}/v8")
set(v8_BUILD_DIR "${v8_ROOT}/v8/out.gn/${v8_ARCH}.${CMAKE_BUILD_TYPE}")

# Cmake build directory
set(v8_INSTALL_DIR "${CMAKE_BINARY_DIR}/v8-install")
set(v8_BINARY_DIR "${v8_INSTALL_DIR}/lib")
set(v8_INCLUDE_DIRS "${v8_INSTALL_DIR}/include")
set(v8_LIBRARIES "${v8_BINARY_DIR}/libv8_monolith.a")
set(v8_BUILD_SCRIPT "${CMAKE_SOURCE_DIR}/sh/build-v8.sh")

if(NOT EXISTS "${v8_LIBRARIES}")

    message(STATUS "${v8_LIBRARIES}} not found. Running build script...")

    execute_process(
        COMMAND ${CMAKE_COMMAND} -E env "PATH=$ENV{PATH}"
                bash "${v8_BUILD_SCRIPT}"
                "${CMAKE_BUILD_TYPE}"
                "${CMAKE_SYSTEM_NAME}"
                "${v8_ARCH}"
                "${v8_ROOT}"
                "${v8_BUILD_DIR}"
                "${v8_INSTALL_DIR}"
                "${LIB3_V8_VERSION}"
        RESULT_VARIABLE result
    )

    if(NOT result EQUAL 0)
        message(FATAL_ERROR "Failed to build V8. See output above.")
    endif()
endif()

# Expose libv8_monolith.a as an imported target so dependents can link it
if(NOT TARGET v8)
    add_library(v8 STATIC IMPORTED GLOBAL)
    set_target_properties(v8 PROPERTIES
        IMPORTED_LOCATION "${v8_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${v8_INCLUDE_DIRS}"
    )
endif()

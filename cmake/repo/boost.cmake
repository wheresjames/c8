
#====================================================================

include(FetchContent)

FetchContent_Declare(
        boost
        GIT_REPOSITORY      https://github.com/boostorg/boost.git
        GIT_TAG             ${LIB3_BOOST_VERSION}
        GIT_PROGRESS        TRUE
    )
FetchContent_MakeAvailable(boost)


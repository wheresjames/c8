#pragma once

//===================================================================
// Static or shared library

// Default to static
#if !defined(C8_SHARED) && !defined(C8_EXPORTS)
#   define C8_STATIC
#endif

#if defined(C8_STATIC) && defined(C8_EXPORTS)
#   error "C8_STATIC and C8_EXPORTS cannot be defined at the same time"
#endif

#if defined(C8_STATIC)
#   define C8_LIB_TYPE "static"
#elif defined(C8_EXPORTS)
#   define C8_LIB_TYPE "shared"
#elif defined(C8_SHARED)
#   define C8_LIB_TYPE "shared"
#else
#   define C8_LIB_TYPE "unknown"
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
#   ifdef C8_STATIC
#       define C8_API __declspec(dllexport)
#   else
#       ifdef C8_EXPORTS
#           define C8_API __declspec(dllexport)
#       else
#           define C8_API __declspec(dllimport)
#       endif
#   endif
#else
#   ifdef C8_STATIC
#       define C8_API
#   else
#       define C8_API __attribute__((visibility("default")))
#   endif
#endif

//===================================================================
// Architecture detection

#ifdef _WIN32
#   ifdef _WIN64
#       define C8_ARCH "x86_64"
#   else
#       define C8_ARCH "x86"
#   endif

#elif defined(__x86_64__) || defined(__ppc64__)
#   define C8_ARCH "x86_64"
#elif defined(__i386__) || defined(__i686__)
#   define C8_ARCH "x86"
#elif defined(__aarch64__)
#   define C8_ARCH "aarch64"
#elif defined(__arm__)
#   ifdef __ARM_ARCH_8__
#       define C8_ARCH "armv8"
#   elif defined(__ARM_ARCH_7__)
#       define C8_ARCH "armv7"
#   elif defined(__ARM_ARCH_6__)
#       define C8_ARCH "armv6"
#   else
#       define C8_ARCH "arm"
#   endif
#else
#   error "Unsupported architecture!"
#endif


//===================================================================
// Platform detection

// Windows?
#ifdef _WIN32
#   define C8_PLATFORM "windows"
#   define C8_WINDOWS

// Apple?
#elif defined(__APPLE__)
#   define C8_PLATFORM "apple"
#   define C8_APPLE
#   define C8_POSIX
#   if defined(__MACH__) && defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
#       define C8_IOS
#   else
#       define C8_MACOS
#   endif

// Android?
#elif defined(__ANDROID__)
#   define C8_PLATFORM "android"
#   define C8_ANDROID
#   define C8_POSIX

// Linux?
#elif defined(__linux__)
#   define C8_PLATFORM "linux"
#   define C8_LINUX
#   define C8_POSIX

// Posix?
#elif defined(__unix__) || defined(__unix)
#   define C8_PLATFORM "posix"
#   define C8_UNIX
#   define C8_POSIX

// Hmmm
#else
#   error "Unsupported platform!"
#endif

// String definitions for platform, architecture, and library type
#define C8_PLATFORM_STRING  C8_PLATFORM \
                            "-" C8_ARCH \
                            "-" C8_LIB_TYPE

/** Returns a string representing the platform and architecture.
    @note The format is "platform-architecture-library_type", e.g., "linux-x86_64-shared".
*/
C8_API const char* c8_getPlatformString();

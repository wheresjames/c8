#pragma once

//===================================================================
// API
#include "c8.h"

//===================================================================
// System
#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
#include <algorithm>
#include <random>
#include <fstream>
#include <filesystem>
#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <map>
#include <list>
#include <any>

/// Namespace
#define C8_NAMESPACE c8
#define C8_MAKENAME(name) c8_##name
#define C8_LOGLEVEL(name) C8_MAKENAME(LogLevel_##name)

/// Log function pointer
typedef void (*C8_MAKENAME(LogFunctionPtr))(int level, const char *msg, const char *file, int line, const char *func);

extern "C" {

    /// Error message function
    std::string C8_MAKENAME(ErrMsg)(const std::string &sMsg, const char *file, const int line, const char *func);

    /// Log function
    int C8_MAKENAME(LogMsg)(const int level, const char *msg, const char *file, const int line, const char *func);
}

/// Log levels
typedef enum {
    C8_LOGLEVEL(None) = 0,
    C8_LOGLEVEL(Fatal),
    C8_LOGLEVEL(Error),
    C8_LOGLEVEL(Warning),
    C8_LOGLEVEL(Info),
    C8_LOGLEVEL(Debug),
} C8_MAKENAME(LogLevel);

/// Logging macros
#define c8_Log(level, ...) \
    c8_LogMsg(level, C8_NAMESPACE::join(" ", __VA_ARGS__).c_str(), __FILE__, __LINE__, __FUNCTION__)
#define c8_LogFatal(...) c8_Log(C8_LOGLEVEL(Fatal), __VA_ARGS__)
#define c8_LogError(...) c8_Log(C8_LOGLEVEL(Error), __VA_ARGS__)
#define c8_LogWarning(...) c8_Log(C8_LOGLEVEL(Warning), __VA_ARGS__)
#define c8_LogInfo(...) c8_Log(C8_LOGLEVEL(Info), __VA_ARGS__)
#define c8_LogDebug(...) c8_Log(C8_LOGLEVEL(Debug), __VA_ARGS__)



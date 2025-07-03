#pragma once


/// Log function pointer
typedef void (*c8_LogFunctionPtr)(int level, const char *msg, const char *file, int line, const char *func);

/// Level name helper
#define C8_LOGLEVEL(name) c8_LogLevel_##name

/// Log levels
typedef enum {
    C8_LOGLEVEL(None) = 0,
    C8_LOGLEVEL(Fatal),
    C8_LOGLEVEL(Error),
    C8_LOGLEVEL(Warning),
    C8_LOGLEVEL(Info),
    C8_LOGLEVEL(Debug),
} c8_LogLevel;

/// Logging macros
#define c8_Log(level, ...) \
    c8_LogMsg(level, c8_join(" ", __VA_ARGS__).c_str(), __FILE__, __LINE__, __FUNCTION__)
#define c8_LogFatal(...) c8_Log(C8_LOGLEVEL(Fatal), __VA_ARGS__)
#define c8_LogError(...) c8_Log(C8_LOGLEVEL(Error), __VA_ARGS__)
#define c8_LogWarning(...) c8_Log(C8_LOGLEVEL(Warning), __VA_ARGS__)
#define c8_LogInfo(...) c8_Log(C8_LOGLEVEL(Info), __VA_ARGS__)
#define c8_LogDebug(...) c8_Log(C8_LOGLEVEL(Debug), __VA_ARGS__)

/// Error message function
std::string c8_ErrMsg(const std::string &sMsg, const char *file, const int line, const char *func);

/// Log function
int c8_LogMsg(const int level, const char *msg, const char *file, const int line, const char *func);



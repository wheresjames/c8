
#include "c8/c8-internal.h"


#if defined(C8_POSIX)

static void signal_handler(int signum)
{
    c8_LogFatal("Caught signal ", signum);
    c8_logBacktrace();
    std::_Exit(signum);  // Don't call destructors
}

static std::terminate_handler old_terminate_handler = nullptr;
static __sighandler_t old_signal_handlers[5]
    = { nullptr, nullptr, nullptr, nullptr, nullptr };

C8_API void c8_installCrashHandlers()
{
    if (old_signal_handlers[0] != nullptr) {
        c8_LogWarning("Crash handlers already installed.");
        return;
    }

    old_signal_handlers[0] = std::signal(SIGSEGV, signal_handler);
    old_signal_handlers[1] = std::signal(SIGABRT, signal_handler);
    old_signal_handlers[2] = std::signal(SIGFPE,  signal_handler);
    old_signal_handlers[3] = std::signal(SIGILL,  signal_handler);
    old_signal_handlers[4] = std::signal(SIGTERM, signal_handler);

    old_terminate_handler
        = std::set_terminate([]() {
            c8_LogFatal("Unhandled exception or std::terminate()!");
            c8_logBacktrace();
            std::_Exit(1);
        });
}

C8_API void c8_removeCrashHandlers()
{
    if (old_signal_handlers[0] == nullptr) {
        c8_LogWarning("Crash handlers were not installed.");
        return;
    }

    // Restore old signal handlers
    std::signal(SIGSEGV, old_signal_handlers[0]);
    std::signal(SIGABRT, old_signal_handlers[1]);
    std::signal(SIGFPE,  old_signal_handlers[2]);
    std::signal(SIGILL,  old_signal_handlers[3]);
    std::signal(SIGTERM, old_signal_handlers[4]);
    old_signal_handlers[0] = nullptr;

    if (old_terminate_handler != nullptr) {
        std::set_terminate(old_terminate_handler);
        old_terminate_handler = nullptr;
    }
}

#elif defined(C8_WINDOWS)

// +++ Windows code as of yet untested

#include <windows.h>

static std::terminate_handler old_terminate_handler = nullptr;
static LONG WINAPI (*old_crash_handler)(EXCEPTION_POINTERS*) = nullptr;

static LONG WINAPI crash_handler(EXCEPTION_POINTERS* pExceptionInfo)
{
    c8_LogFatal("Caught exception: ", pExceptionInfo->ExceptionRecord->ExceptionCode);
    c8_logBacktrace();
    return EXCEPTION_EXECUTE_HANDLER;
}

C8_API void c8_installCrashHandlers()
{
    if (old_crash_handler != nullptr) {
        c8_LogWarning("Crash handlers already installed.");
        return;
    }

    old_crash_handler = SetUnhandledExceptionFilter(crash_handler);

    old_terminate_handler
        = std::set_terminate([]() {
            c8_LogFatal("Unhandled exception or std::terminate()!");
            c8_logBacktrace();
            std::_Exit(1);
        });
}

C8_API void c8_removeCrashHandlers()
{
    if (old_crash_handler == nullptr) {
       c8_LogWarning("Crash handlers were not installed.");
        return;
    }

    SetUnhandledExceptionFilter(old_crash_handler);
    old_crash_handler = nullptr;

    if (old_terminate_handler != nullptr) {
        std::set_terminate(old_terminate_handler);
        old_terminate_handler = nullptr;
    }
}

#endif


C8_API void c8_logBacktrace()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::string backTrace = "\n" + c8_getStacktraceString();

    c8_LogError("Crash! Stack trace: ",
                std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S"),
                backTrace);

    // Build path to crash log file
    std::string exePath = c8_getExePath();
    std::string exeDir = std::filesystem::path(exePath).parent_path().string();
    std::string crashLogPath = exeDir.empty() ? "crash.log" : exeDir + "/crash.log";
    std::ofstream out(crashLogPath, std::ios::app);
    if (out)
    {   c8_LogInfo("Writing crash stack trace to: ", crashLogPath);
        out << std::endl
            << "==== Crash Stack Trace "
            << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S")
            << " ====\n"
            << backTrace;
    }

}

C8_API std::string c8_getStacktraceString()
{
    std::ostringstream oss;
    boost::stacktrace::stacktrace st;
    for (std::size_t i = 0; i < st.size(); ++i)
    {
        const auto& frame = st[i];
        oss << "#" << i
            << " " << frame.address()
            << " " << frame.name();

        if (!frame.source_file().empty())
            oss << " " << frame.source_file()
                << ":" << frame.source_line()
                << std::endl;
        else
            oss << std::endl;
    }

    return oss.str();
}



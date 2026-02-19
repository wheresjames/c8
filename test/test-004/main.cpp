/**
    This is the sample program from the V8 documentation.
*/

#include <c8.h>
#include <iostream>

#if defined(C8_WINDOWS)
int crashApp()
{
    __debugbreak();
    return 0;
}
#else
__attribute__((noinline))
int crashApp()
{
    raise(SIGSEGV);
    return 0;
}
#endif

int main(int argc, char* argv[])
{
    std::cout << "EXE: " << c8_getExePath() << std::endl;

    std::cout << "Crash test application." << std::endl;

    std::cout << "Installing signal handlers..." << std::endl;
    c8_installCrashHandlers();

    std::cout << "Backtrace before crash:" << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << c8_getStacktraceString() << std::endl;
    std::cout << "============================" << std::endl;

    std::cout << "Crashing the application..." << std::endl;
    crashApp();

    std::cout << "App did not crash..." << std::endl;

    return 0;
}

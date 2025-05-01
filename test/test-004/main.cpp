/**
    This is the sample program from the V8 documentation.
*/

#include <iostream>

#define BOOST_STACKTRACE_USE_BACKTRACE
#include <boost/stacktrace.hpp>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <fstream>

void write_stacktrace()
{
    std::ofstream out("crash.log");
    if (out)
    {
        out << "==== Crash Stack Trace ====\n";
        out << boost::stacktrace::stacktrace();
    }

    std::cerr << "Crash! Stack trace:\n";
    std::cerr << boost::stacktrace::stacktrace();
}

void signal_handler(int signum)
{
    std::cerr << "\nCaught signal " << signum << "\n";
    write_stacktrace();
    std::_Exit(signum);  // avoid calling destructors
}

void install_handlers()
{
    std::signal(SIGSEGV, signal_handler);
    std::signal(SIGABRT, signal_handler);
    std::signal(SIGFPE,  signal_handler);
    std::signal(SIGILL,  signal_handler);
    std::signal(SIGTERM, signal_handler);

    std::set_terminate([]() {
        std::cerr << "Unhandled exception or std::terminate()!\n";
        write_stacktrace();
        std::_Exit(1);
    });
}

int crashApp()
{
    int* p = nullptr;
    *p = 42;
    return 0;
}

int main(int argc, char* argv[])
{
    std::cout << "Crash test application." << std::endl;

//    std::cerr << "Backend: " << BOOST_STACKTRACE_BACKEND << "\n";

    std::cout << "Installing signal handlers..." << std::endl;
    install_handlers();

    std::cout << "Backtrace before crash:" << std::endl;
    std::cout << "============================" << std::endl;
    boost::stacktrace::stacktrace st;
    for (std::size_t i = 0; i < st.size(); ++i)
    {
        const auto& frame = st[i];
        std::cout << "#" << i
                << " " << frame.name()
                << " at " << frame.source_file()
                << ":" << frame.source_line() << std::endl;
    }
    std::cout << "============================" << std::endl;

    std::cout << "Crashing the application..." << std::endl;
    crashApp();

    std::cout << "App did not crash..." << std::endl;

    return 0;
}

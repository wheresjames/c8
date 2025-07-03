
#include "c8/c8-internal.h"

//====================================================================
// Windows
#ifdef C8_WINDOWS

#include <windows.h>

C8_API std::string c8_getExePath()
{
    char buf[MAX_PATH];
    DWORD len = GetModuleFileNameA(NULL, buf, MAX_PATH);
    return (len > 0) ? std::string(buf, len) : "";
}


#elif defined(C8_POSIX)

#include <unistd.h>
#include <limits.h>

C8_API std::string c8_getExePath()
{
    std::vector<char> buf(PATH_MAX);
    ssize_t len = readlink("/proc/self/exe", buf.data(), buf.size() - 1);
    if (len != -1)
    {
        buf[len] = '\0';
        return std::string(buf.data());
    }
    return "";
}

#elif defined(C8_MACOS)

#include <mach-o/dyld.h>

C8_API std::string c8_getExePath()
{
    uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size);
    std::vector<char> buf(size + 1);
    if (_NSGetExecutablePath(buf.data(), &size) == 0)
    {   buf.data()[size] = '\0';
        return std::string(buf.data());
    }
    return "";
}

#else

C8_API std::string c8_getExePath()
{
    return "";
}

#endif


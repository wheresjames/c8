
#include "c8/c8-internal.h"

//=============================================================================
// Global variables
static struct {

    /// False if not initialized
    bool                                                        bInit = false;

    /// Log level
    int                                                         nLogLevel = C8_LOGLEVEL(Warning);

    /// Callback function for logging
    c8_LogFunctionPtr                                           onLogMessage = 0;

} c8_globals;


//=============================================================================
// Log functions

std::string c8_ErrMsg(const std::string &sMsg, const char *file, const int line, const char *func)
{
    if (file)
#ifdef __APPLE__
        {
            std::string sFile = file;
            size_t nPos = sFile.find_last_of('/');
            if (nPos != std::string::npos)
                sFile = sFile.substr(nPos + 1);
            return std::string(".../") + sFile + ":" + std::to_string(line) + ": " + sMsg;
        }

#else
        return std::string(".../") + std::filesystem::path(file).filename().string() + ":" + std::to_string(line) + ": " + sMsg;
#endif
    else
        return sMsg;
}

int c8_LogMsg(const int level, const char *msg, const char *file, const int line, const char *func)
{
    if (level > c8_globals.nLogLevel)
        return 0;

    if (!msg)
        return -1;

    try
    {
        std::string sLevel = "[UNKWN] ";
        switch (level)
        {   default: break;
            case C8_LOGLEVEL(Fatal): sLevel = "[FATAL] "; break;
            case C8_LOGLEVEL(Error): sLevel = "[ERROR] "; break;
            case C8_LOGLEVEL(Warning): sLevel = "[WARN ] "; break;
            case C8_LOGLEVEL(Info): sLevel = "[INFO ] "; break;
            case C8_LOGLEVEL(Debug): sLevel = "[DEBUG] "; break;
        }

        // Get timestamp
        auto now = std::chrono::system_clock::now();
        std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *std::localtime(&now_time_t);
        std::stringstream ss;
        ss << std::put_time(&now_tm, "[%H:%M:%S] ");

        std::string sDescription = ss.str() + c8_ErrMsg(msg, file, line, func);

        // STDERR?
        if (!c8_globals.onLogMessage)
            std::cerr << sDescription << std::endl;

        // Use callback function
        else
            c8_globals.onLogMessage(level, msg, file, line, func);
    }
    catch (std::exception &e)
    {   std::cerr << file << ":" << line << ": Error in c8_LogMsg: " << e.what() << std::endl;
    }
    catch (...)
    {   std::cerr << file << ":" << line << ": Error in c8_LogMsg" << std::endl;
    }

    return 0;
}

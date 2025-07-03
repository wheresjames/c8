#pragma once

struct C8_GLOBALS
{

    /// False if not initialized
    bool                                                        bInit = false;

    /// Log level
    int                                                         nLogLevel = C8_LOGLEVEL(Warning);

    /// Callback function for logging
    c8_LogFunctionPtr                                           onLogMessage = 0;

};

/// Global variables
extern C8_GLOBALS c8_globals;

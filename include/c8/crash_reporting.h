#pragma once

/** Installs signal handlers for crash reporting.

   This function sets up handlers for various signals such as SIGSEGV, SIGABRT,
   SIGFPE, SIGILL, and SIGTERM to log stack traces and exit gracefully on crashes.
   It also sets a custom terminate handler for unhandled exceptions.
*/
C8_API void c8_installCrashHandlers();

/** Removes the installed crash handlers.

   This function restores the default signal handlers for the signals that were
   previously handled by `c8_installCrashHandlers`. It is useful for cleanup
   when crash reporting is no longer needed.
*/
C8_API void c8_removeCrashHandlers();


/** Returns a string containing the current backtrace.

   This function captures the current stack trace and returns it as a formatted string.
   It is useful for debugging purposes, especially in the event of a crash.

   @return A string representation of the current backtrace.
 */
C8_API std::string c8_getStacktraceString();

/** Logs the current backtrace to the standard error output and optionally to a crash log file.

   This function captures the current stack trace, formats it, and prints it to the
   standard error output. It also writes the backtrace to a log file named "crash.log"
   in the same directory as the executable, if possible.

   @note This function is typically called in response to a crash or unhandled exception.
*/
C8_API void c8_logBacktrace();

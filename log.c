#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "log.h"

enum log_type LOG_LEVEL = LOG_INFO;
FILE* LOG_FILE;

int log_open(const char* filepath, enum log_type level)
{
        LOG_LEVEL = level;

        LOG_FILE = fopen(filepath, "a");
        if (!LOG_FILE) {
                fprintf(stderr, "ERROR: Could not open log file %s\n",
                                filepath);
                return -1;
        }
        return 0;
}

void log_close(void)
{
        if (!LOG_FILE)
                fprintf(stderr,
                       "ERROR: Did not attempt to close log file."
                       " File is not open.\n"); 
        fclose(LOG_FILE);
}

static const char* log_type_str(enum log_type log_type)
{
        switch (log_type) {
        case LOG_DEBUG: return "DEBUG";
        default:
        case LOG_INFO: return "INFO";
        case LOG_WARN: return "WARN";
        case LOG_ERROR: return "ERROR";
        }
}

static void get_now_str(char* buf, size_t bufsize)
{
        time_t now = time(NULL);
        struct tm* p = localtime(&now);
        strftime(buf, bufsize, "%Y-%m-%d %H:%M:%S", p);
}

static bool _log(enum log_type log_type, const char* format, va_list argptr)
{
        bool ret = true;
        if (LOG_LEVEL > log_type || !LOG_FILE) {
                ret = false;
                goto end;
        }

        char datestr[256];
        get_now_str(datestr, 256);

        if (log_type >= LOG_WARN)
                vfprintf(stdout, format, argptr);
        if (log_type == LOG_ERROR)
                vfprintf(stderr, format, argptr);

        fprintf(LOG_FILE, "%s %s - ", datestr, log_type_str(log_type));

        vfprintf(LOG_FILE, format, argptr);
end:
        va_end(argptr);
        return ret;
}

bool log_debug(const char* format, ...)
{
        va_list argptr;
        va_start(argptr, format);
        return _log(LOG_DEBUG, format, argptr);
}

bool log_info(const char* format, ...)
{
        va_list argptr;
        va_start(argptr, format);
        return _log(LOG_INFO, format, argptr);
}

bool log_warn(const char* format, ...)
{
        va_list argptr;
        va_start(argptr, format);
        return _log(LOG_WARN, format, argptr);
}

bool log_error(const char* format, ...)
{
        va_list argptr;
        va_start(argptr, format);
        return _log(LOG_ERROR, format, argptr);
}


#include <stdarg.h>
#include <stdbool.h>

enum log_type { LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR }; /* Order matters! */

int log_open(const char* filepath, enum log_type level);
void log_close(void);

bool log_debug(const char* format, ...);
bool log_info(const char* format, ...);
bool log_warn(const char* format, ...);
bool log_error(const char* format, ...);


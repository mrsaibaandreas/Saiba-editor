#pragma once
#include <iostream>
#include <stdarg.h>
enum class LogLevel {
    #ifdef DEBUG_A
    DEBUG,
    #endif
    INFO,
    WARNING,
    ERROR
    
};

#ifdef DEBUG_A
    #define LOG_D(fmt, ...) log_msg(LogLevel::DEBUG, fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define LOG_D(...)
#endif

#define LOG_I(level, fmt, ...) log_msg(level, fmt, __FILE__, __LINE__, ##__VA_ARGS__)

inline void log_msg(LogLevel level, const char *fmt, const char *file, int line, ...) {
    
    switch(level) {
        #ifdef DEBUG_A
        case LogLevel::DEBUG:
            fprintf(stderr, "[DEBUG] [%s (%i)] ", file, line);
            break;
        #endif
        case LogLevel::INFO:
            fprintf(stderr, "[INFO] [%s (%i)] ", file, line);
            break;
        case LogLevel::WARNING:
            fprintf(stderr, "[WARNING] [%s (%i)] ", file, line);
            break;
        case LogLevel::ERROR:
            fprintf(stderr, "[ERROR] [%s (%i)] ", file, line);
            break;
        default: 
            fprintf(stderr, "Unknown log level, not able to print\n");
    }
    
    va_list args;
    va_start(args, line);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");    
}

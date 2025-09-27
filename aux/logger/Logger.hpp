#pragma once
#include <iostream>
#include <ctime>
#include <string>
#include <stdarg.h>
#include <errno.h>
enum class LogLevel {
    #ifdef DEBUG_A
    DEBUG,
    #endif
    INFO,
    WARNING,
    ERROR
    
};

inline FILE *LOG_FILE;
inline void INIT_LOG(const std::string& fileName) {
    std::time_t  time = std::time({});
    char timeString[std::size("yyyy-mm-dd--hh:mm:ss")];
    std::strftime(std::data(timeString), std::size(timeString), "%FT%TZ", std::gmtime(&time));
    
    std::string logFileTimeStamped = fileName + timeString;
    LOG_FILE = fopen(logFileTimeStamped.c_str(), "w+"); 

    if (LOG_FILE == NULL) {
        perror("Error opening file");
        std::abort(); 
    }
}
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
            fprintf(LOG_FILE, "[DEBUG] [%s (%i)] ", file, line);
            break;
        #endif
        case LogLevel::INFO:
            fprintf(LOG_FILE, "[INFO] [%s (%i)] ", file, line);
            break;
        case LogLevel::WARNING:
            fprintf(LOG_FILE, "[WARNING] [%s (%i)] ", file, line);
            break;
        case LogLevel::ERROR:
            fprintf(LOG_FILE, "[ERROR] [%s (%i)] ", file, line);
            break;
        default: 
            fprintf(LOG_FILE, "Unknown log level, not able to print\n");
    }
    
    va_list args;
    va_start(args, line);
    vfprintf(LOG_FILE, fmt, args);
    va_end(args);
    fprintf(LOG_FILE, "\n");    
}

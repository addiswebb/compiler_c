#ifndef COMPILER_C_LOGGER_H
#define COMPILER_C_LOGGER_H

#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_PANIC,
} LogLevel;

typedef enum {
    STAGE_COMPILER,
    STAGE_TOKENIZING,
    STAGE_PARSING,
    STAGE_SEMA_ANALYSIS,
    STAGE_IR,
    STAGE_X86_GEN,
} LogStage;

typedef struct {
    LogLevel min_level;
    LogStage stage;
    FILE *file;
} Logger;

extern Logger logger;


static inline void init_logger(FILE *fp, LogLevel level) {
    logger.file = fp ? fp : stderr;
    logger.stage = STAGE_COMPILER;
    logger.min_level = level;
}
static inline void set_log_stage(LogStage stage) { logger.stage = stage; }

static inline void log_start(LogLevel lvl){
    if (lvl < logger.min_level) return;
    const char *level_str;
    switch (lvl) {
    case LOG_DEBUG:
        level_str = "\x1b[34mDEBUG\x1b[0m";
        break;
    case LOG_INFO:
        level_str = "\x1b[32mINFO\x1b[0m";
        break;
    case LOG_WARN:
        level_str = "\x1b[33mWARN\x1b[0m";
        break;
    case LOG_ERROR:
        level_str = "\x1b[31mERROR\x1b[0m";
        break;
    case LOG_PANIC:
        level_str = "\x1b[1;31mPANIC\x1b[0m";
        break;
    default:
        level_str = "LOG";
        break;
    }
    const char *stage_str;
    switch (logger.stage) {
    case STAGE_COMPILER:
        stage_str = "Compiler";
        break;
    case STAGE_TOKENIZING:
        stage_str = "Tokenizer";
        break;
    case STAGE_PARSING:
        stage_str = "Parser";
        break;
    case STAGE_IR:
        stage_str = "IR";
        break;
    case STAGE_X86_GEN:
        stage_str = "x86 Gen";
        break;
    case STAGE_SEMA_ANALYSIS:
        stage_str = "Semantic Analysis";
        break;
    }
    fprintf(logger.file, "[%s] %s: ", level_str, stage_str);
}

static inline void log_message(LogLevel lvl, const char *fmt, ...) {
    if (lvl < logger.min_level) return;
    log_start(lvl);
    va_list args;
    va_start(args, fmt);
    vfprintf(logger.file, fmt, args);
    va_end(args);
    fflush(logger.file);
    if(lvl == LOG_PANIC) exit(1);
}
#define DEBUG(fmt, ...) log_message(LOG_DEBUG, fmt, ##__VA_ARGS__)
#define INFO(fmt, ...) log_message(LOG_INFO, fmt, ##__VA_ARGS__)
#define WARN(fmt, ...) log_message(LOG_WARN, fmt, ##__VA_ARGS__)
#define ERROR(fmt, ...) log_message(LOG_ERROR, fmt, ##__VA_ARGS__)
#define PANIC(fmt, ...) do { \
    log_message(LOG_ERROR, fmt, ##__VA_ARGS__); \
    exit(1); \
} while(0)

#define ASSERT(cond, fmt, ...) if (!(cond)) PANIC(fmt, ##__VA_ARGS__)

#endif // COMPILER_C_LOGGER_H

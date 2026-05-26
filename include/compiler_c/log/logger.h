#ifndef COMPILER_C_LOGGER_H
#define COMPILER_C_LOGGER_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// extern void *free_check;
// #define free(ptr) \
//     do { \
//         if (free_check && ptr == free_check) PANIC("Double free %p\n", ptr); \
//         free(ptr); \
//     } while (0)

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
    STAGE_ASSEMBLER,
    STAGE_LINKER
} LogStage;

typedef struct {
    LogLevel min_level;
    LogStage stage;
    FILE *file;
} Logger;

extern Logger logger;

#ifdef __COMPILER_C__
#define PRINTCC(s) printf("[CC] " s "\n");
#define exit_bp() exit(1)
#else
// extern void exit_bp() __THROW __attribute__((__noreturn__));

__attribute__((noreturn)) static inline void exit_bp() { exit(1); }
#define PRINTCC(s) ((void)0)
#endif

static inline void init_logger(FILE *fp, LogLevel level) {
    logger.file = fp ? fp : stderr;
    logger.stage = STAGE_COMPILER;
    logger.min_level = level;
}

static inline char *stage_str(LogStage stage) {
    switch (logger.stage) {
    case STAGE_COMPILER:
        return "Compiler";
    case STAGE_TOKENIZING:
        return "Tokenizer";
    case STAGE_PARSING:
        return "Parser";
    case STAGE_IR:
        return "IR";
    case STAGE_X86_GEN:
        return "x86 Gen";
    case STAGE_SEMA_ANALYSIS:
        return "Semantic Analysis";
    case STAGE_ASSEMBLER:
        return "Assembler";
    case STAGE_LINKER:
        return "Linker";
    }
}
static inline void set_log_stage(LogStage stage) { logger.stage = stage; }

static inline void log_start(LogLevel lvl) {
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
    const char *stage = stage_str(logger.stage);
    fprintf(logger.file, "[%s] %s: ", level_str, stage);
}
void print(const char *fmt, ...);
void vprint(const char *fmt, va_list ap);

static inline void log_message(LogLevel lvl, const char *fmt, ...) {
    if (lvl < logger.min_level) return;
    log_start(lvl);
    va_list args;
    va_start(args, fmt);
    vprint(fmt, args);
    va_end(args);
    fflush(logger.file);
    if (lvl == LOG_PANIC) exit_bp();
}

#define DEBUG(fmt, ...) log_message(LOG_DEBUG, fmt, ##__VA_ARGS__)
#define INFO(fmt, ...) log_message(LOG_INFO, fmt, ##__VA_ARGS__)
#define WARN(fmt, ...) log_message(LOG_WARN, fmt, ##__VA_ARGS__)
#define ERROR(fmt, ...) log_message(LOG_ERROR, fmt, ##__VA_ARGS__)
#define PANIC(fmt, ...)                                                                                                                    \
    do {                                                                                                                                   \
        log_message(LOG_ERROR, fmt, ##__VA_ARGS__);                                                                                        \
        exit_bp();                                                                                                                         \
    } while (0)

#ifdef __COMPILER_C__
#define ASSERT(cond, fmt, ...)                                                                                                             \
    if (!(cond)) PANIC(fmt, ##__VA_ARGS__)
#else
#define ASSERT(cond, fmt, ...)                                                                                                             \
    if (__builtin_expect(!(cond), 0)) PANIC(fmt, ##__VA_ARGS__)
#endif

#define ASSERT_DB(cond, fmt, ...)                                                                                                          \
    if (!(cond)) PANIC("[" #fmt "]" fmt, ##__VA_ARGS__)

#endif // COMPILER_C_LOGGER_H

cmake_minimum_required(VERSION 3.28.3)
project(compiler_c C)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

if(WIN32)
    set(ABI_SRC src/abi/win64.c)
else()
    set(ABI_SRC src/abi/sysv.c)
endif()

add_compile_definitions(LIBC=${CMAKE_SOURCE_DIR}/libc)

add_executable(compiler_c
    src/main.c
    src/compiler.c
    src/core/node.c
    src/core/util.c
    src/core/array.c
    src/core/type.c
    src/core/arena.c
    src/tokenize/tokenizer.c
    src/parse/parser.c
    src/x86/x86_gen.c
    src/x86/x86_emit.c
    src/ir/ir_gen.c
    src/ir/ir_util.c
    src/ir/ir_builder.c
    src/ir/ir_module.c
    src/analyse/sema.c
    src/analyse/analysis.c
    src/analyse/const_expr.c
    ${ABI_SRC}
    src/log/logger.c
)

target_include_directories(compiler_c
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
